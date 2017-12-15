#include <kernel/mem.h>
#include <kernel/atag.h>
#include <common/stdlib.h>
#include <stdint.h>
#include <stddef.h>

/**
 * Heap Stuff
 */
static void heap_init(uint32_t heap_start);
/**
 * impliment kmalloc as a linked list of allocated segments.
 * Segments should be 4 byte aligned.
 * Use best fit algorithm to find an allocation
 */
typedef struct heap_segment{
    struct heap_segment * next;
    struct heap_segment * prev;
    uint32_t is_allocated;
    uint32_t segment_size;  // Includes this header
} heap_segment_t;

static heap_segment_t * heap_segment_list_head;

/**
 * End Heap Stuff
 */


extern uint8_t __end;
static uint32_t num_pages;

DEFINE_LIST(page);
IMPLEMENT_LIST(page);

static page_t * all_pages_array;
page_list_t free_pages;



void mem_init(atag_t * atags) {
    uint32_t mem_size, page_array_len, kernel_pages, page_array_end, i;

    // Get the total number of pages
    mem_size = get_mem_size(atags);
    num_pages = mem_size / PAGE_SIZE;

    // Allocate space for all those pages' metadata.  Start this block just after the kernel image is finished
    page_array_len = sizeof(page_t) * num_pages;
    all_pages_array = (page_t *)&__end;
    bzero(all_pages_array, page_array_len);
    INITIALIZE_LIST(free_pages);

    // Iterate over all pages and mark them with the appropriate flags
    // Start with kernel pages
    kernel_pages = ((uint32_t)&__end) / PAGE_SIZE;
    for (i = 0; i < kernel_pages; i++) {
        all_pages_array[i].vaddr_mapped = i * PAGE_SIZE;    // Identity map the kernel pages
        all_pages_array[i].flags.allocated = 1;
        all_pages_array[i].flags.kernel_page = 1;
    }
    // Reserve 1 MB for the kernel heap
    for(; i < kernel_pages + (KERNEL_HEAP_SIZE / PAGE_SIZE); i++){
        all_pages_array[i].vaddr_mapped = i * PAGE_SIZE;    // Identity map the kernel pages
        all_pages_array[i].flags.allocated = 1;
        all_pages_array[i].flags.kernel_heap_page = 1;
    }
    // Map the rest of the pages as unallocated, and add them to the free list
    for(; i < num_pages; i++){
        all_pages_array[i].flags.allocated = 0;
        append_page_list(&free_pages, &all_pages_array[i]);
    }


    // Initialize the heap
    page_array_end = (uint32_t)&__end + page_array_len;
    heap_init(page_array_end);

}

void * alloc_page(void) {
    page_t * page;
    void * page_mem;


    if (size_page_list(&free_pages) == 0)
        return 0;

    // Get a free page
    page = pop_page_list(&free_pages);
    page->flags.kernel_page = 1;
    page->flags.allocated = 1;

    // Get the address the physical page metadata refers to
    page_mem = (void *)((page - all_pages_array) * PAGE_SIZE);

    // Zero out the page, big security flaw to not do this :)
    bzero(page_mem, PAGE_SIZE);

    return page_mem;
}

void free_page(void * ptr) {
    page_t * page;

    // Get page metadata from the physical address
    page = all_pages_array + ((uint32_t)ptr / PAGE_SIZE);

    // Mark the page as free
    page->flags.allocated = 0;
    append_page_list(&free_pages, page);
}


static void heap_init(uint32_t heap_start) {
   heap_segment_list_head = (heap_segment_t *) heap_start;
   bzero(heap_segment_list_head, sizeof(heap_segment_t));
   heap_segment_list_head->segment_size = KERNEL_HEAP_SIZE;
}


void * kmalloc(uint32_t bytes) {
    heap_segment_t * curr, *best = NULL;
    int diff, best_diff = 0x7fffffff; // Max signed int

    // Add the header to the number of bytes we need and make the size 16 byte aligned
    bytes += sizeof(heap_segment_t);
    bytes += bytes % 16 ? 16 - (bytes % 16) : 0;

    // Find the allocation that is closest in size to this request
    for (curr = heap_segment_list_head; curr != NULL; curr = curr->next) {
        diff = curr->segment_size - bytes;
        if (!curr->is_allocated && diff < best_diff && diff >= 0) {
            best = curr;
            best_diff = diff;
        }
    }

    // There must be no free memory right now :(
    if (best == NULL)
        return NULL;

    // If the best difference we could come up with was large, split up this segment into two.
    // Since our segment headers are rather large, the criterion for splitting the segment is that
    // when split, the segment not being requested should be twice a header size
    if (best_diff > (int)(2 * sizeof(heap_segment_t))) {
        bzero(((void*)(best)) + bytes, sizeof(heap_segment_t));
        curr = best->next;
        best->next = ((void*)(best)) + bytes;
        best->next->next = curr;
        best->next->prev = best;
        best->next->segment_size = best->segment_size - bytes;
        best->segment_size = bytes;
    }

    best->is_allocated = 1;

    return best + 1;
}

void kfree(void *ptr) {
    heap_segment_t * seg;

    if (!ptr)
        return;

    seg = ptr - sizeof(heap_segment_t);
    seg->is_allocated = 0;

    // try to coalesce segements to the left
    while(seg->prev != NULL && !seg->prev->is_allocated) {
        seg->prev->next = seg->next;
        seg->next->prev = seg->prev;
        seg->prev->segment_size += seg->segment_size;
        seg = seg->prev;
    }
    // try to coalesce segments to the right
    while(seg->next != NULL && !seg->next->is_allocated) {
        seg->next->next->prev = seg;
        seg->next = seg->next->next;
        seg->segment_size += seg->next->segment_size;
    }
}
