#include <stddef.h>
#include <stdint.h>
#include <kernel/uart.h>
#include <kernel/mem.h>
#include <kernel/atag.h>
#include <kernel/kerio.h>
#include <kernel/gpu.h>
#include <kernel/interrupts.h>
#include <kernel/timer.h>
#include <kernel/process.h>
#include <kernel/mutex.h>
#include <common/stdlib.h>

mutex_t test_mut;

void test(void) {
    int i = 0;
    while (1) {
        if (i % 10 == 0)
            mutex_lock(&test_mut);
        else if (i % 10 == 9) 
            mutex_unlock(&test_mut);
        printf("test %d\n", i++);
        udelay(1000000);
    }
}

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    int i = 0;
    // Declare as unused
    (void) r0;
    (void) r1;
    (void) atags;

    mem_init((atag_t *)atags);
    gpu_init();
    printf("GPU INITIALIZED\n");
    printf("INITIALIZING INTERRUPTS...");
    interrupts_init();
    printf("DONE\n");
    printf("INITIALIZING TIMER...");
    timer_init();
    printf("DONE\n");
    printf("INITIALIZING SCHEDULER...");
    process_init();
    printf("DONE\n");

    puts("Hello, kernel World!\n");

    mutex_init(&test_mut);
    create_kernel_thread(test, "TEST", 4);

    while (1) {
        if (i % 10 == 0)
            mutex_lock(&test_mut);
        else if (i % 10 == 9) 
            mutex_unlock(&test_mut);

        printf("main %d\n", i++);
        udelay(1000000);
    }
}
