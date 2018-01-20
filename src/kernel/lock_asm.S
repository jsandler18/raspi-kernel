.section ".text"

.global try_lock

// This function takes a pointer to a lock variable and uses atomic operations to aqcuire the lock.
// Returns 0 if the lock was not acquired and 1 if it was.
try_lock:
    mov     r1, #0
    swp     r2, r1, [r0]
    mov     r0, r2
    blx     lr 
