.section ".text"

.global move_exception_vector

exception_vector:
    ldr pc, reset_handler_abs_addr
    ldr pc, undefined_instruction_handler_abs_addr
    ldr pc, software_interrupt_handler_abs_addr
    ldr pc, prefetch_abort_handler_abs_addr
    ldr pc, data_abort_handler_abs_addr
    nop                                         // This one is reserved
    ldr pc, irq_handler_abs_addr
    ldr pc, fast_irq_handler_abs_addr

reset_handler_abs_addr:                 .word reset_handler
undefined_instruction_handler_abs_addr: .word undefined_instruction_handler
software_interrupt_handler_abs_addr:    .word software_interrupt_handler
prefetch_abort_handler_abs_addr:        .word prefetch_abort_handler
data_abort_handler_abs_addr:            .word data_abort_handler
irq_handler_abs_addr:                   .word irq_handler_asm_wrapper
fast_irq_handler_abs_addr:              .word fast_irq_handler

move_exception_vector:
    push    {r4, r5, r6, r7, r8, r9}
    ldr     r0, =exception_vector 
    mov     r1, #0x0000
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8}
    pop     {r4, r5, r6, r7, r8, r9}
    blx     lr

irq_handler_asm_wrapper:
    sub     lr, lr, #4      // Adjsut return address
    srsdb   sp!, #0x13      // Save irq lr and irq spsp to supervisor stack, and save the resulting stack pointer as the current stack pointer
    cpsid   if, #0x13       // Switch to supervisor mode with interrupts disabled
    push    {r0-r3, r12, lr}// Save the caller save registers
    and     r1, sp, #4      // Make sure stack is 8 byte aligned
    sub     sp, sp, r1
    push    {r1}            // Save the stack adjustment
    bl      irq_handler
    pop     {r1}            // Get the stack adjustment
    add     sp, sp, r1
    pop     {r0-r3, r12, lr}// Revert the caller save registers
    rfeia   sp!             // Load the saved return address and program state register from before the interrupt from the stack and return 
