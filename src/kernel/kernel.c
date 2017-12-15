#include <stddef.h>
#include <stdint.h>
#include <kernel/uart.h>
#include <kernel/mem.h>
#include <kernel/atag.h>
#include <kernel/kerio.h>
#include <kernel/framebuffer.h>
#include <common/stdlib.h>


void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    char buf[256];
    // Declare as unused
    (void) r0;
    (void) r1;
    (void) atags;

    puts("Initializing Memory Module\n");
    mem_init((atag_t *)atags);
    gpu_init();

    puts("Hello, kernel World!\n");

    while (1) {
        gets(buf,256);
        puts(buf);
        putc('\n');
    }
}
