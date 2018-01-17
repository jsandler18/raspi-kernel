#include <kernel/process.h>
#include <kernel/mem.h>
#include <kernel/interrupts.h>
#include <kernel/timer.h>
#include <common/stdlib.h>

static uint32_t next_proc_num = 1;
#define NEW_PID next_proc_num++;

extern uint8_t __end;
extern void switch_to_thread(process_control_block_t * old, process_control_block_t * new);

IMPLEMENT_LIST(pcb);

pcb_list_t run_queue;
pcb_list_t all_proc_list;

process_control_block_t * current_process;

void schedule(void) {
    DISABLE_INTERRUPTS();
    process_control_block_t * new_thread, * old_thread;

    // If nothing on the run queue, the current process should just continue
    if (size_pcb_list(&run_queue) == 0)
        return;

    // Get the next thread to run.  For now we are using round-robin
    new_thread = pop_pcb_list(&run_queue);
    old_thread = current_process;
    current_process = new_thread;

    // Put the current thread back in the run queue
    append_pcb_list(&run_queue, old_thread);

    // Context Switch
    switch_to_thread(old_thread, new_thread);
    ENABLE_INTERRUPTS();
}

void process_init(void) {
    process_control_block_t * main_pcb;
    INITIALIZE_LIST(run_queue);
    INITIALIZE_LIST(all_proc_list);

    // Allocate and initailize the block
    main_pcb = kmalloc(sizeof(process_control_block_t));
    main_pcb->stack_page = (void *)&__end;
    main_pcb->pid = NEW_PID;
    memcpy(main_pcb->proc_name, "Init", 5);

    // Add self to all process list.  It is already running, so dont add it to the run queue
    append_pcb_list(&all_proc_list, main_pcb);

    current_process = main_pcb;

    // Set the timer to go off after 10 ms
    timer_set(10000);

}

void create_kernel_thread(kthread_function_f thread_func, char * name, int name_len) {
    process_control_block_t * pcb;
    proc_saved_state_t * new_proc_state;

    // Allocate and initialize the pcb
    pcb = kmalloc(sizeof(process_control_block_t));
    pcb->stack_page = alloc_page();
    pcb->pid = NEW_PID;
    memcpy(pcb->proc_name, name, MIN(name_len,19));
    pcb->proc_name[MIN(name_len,19)+1] = 0;

    // Get the location the stack pointer should be in when this is run
    new_proc_state = pcb->stack_page + PAGE_SIZE - sizeof(proc_saved_state_t);
    pcb->saved_state = new_proc_state;

    // Set up the stack that will be restored during a context switch
    bzero(new_proc_state, sizeof(proc_saved_state_t));
    new_proc_state->lr = (uint32_t)thread_func;     // lr is used as return address in switch_to_thread
    new_proc_state->cpsr = 0x13 | (8 << 1);         // Sets the thread up to run in supervisor mode with irqs only

    // add the thread to the lists
    append_pcb_list(&all_proc_list, pcb);
    append_pcb_list(&run_queue, pcb);
}
