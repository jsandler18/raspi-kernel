.section ".text"

.global switch_to_thread

// void switch_to_thread(process_control_block_t * old, process_control_block_t * new)
// takes old thread off cpu and puts new thread on.
// Old thread should be put back in run queue before this is called
switch_to_thread:
    // Save the current thread's state
    push    {lr}     // Push stack pointer and link register
    push    {sp}
    mrs     r12, cpsr   // Get the current program state register
    push    {r0-r12}    // Save all general purpose registers and program state
    str     sp, [r0]    // Store the stack pointer into the saved_state field of the current process
    // Restore the new thread's state
    ldr     sp, [r1]    // Load the stack pointer of the new process
    ldr     r0, =#(10000)// Set Quantum time to 10 ms (10000 us)
    bl      timer_set   // reset the timer to go off in another quantum
    pop     {r0-r12}    // restore the general purpose registers
    msr     cpsr_c, r12   // Restore the new thread's program status register
    pop     {lr, pc}    // we have no idea what lr should be, so just give it a garbage value. pc gets the stored lr so this function returns to there



