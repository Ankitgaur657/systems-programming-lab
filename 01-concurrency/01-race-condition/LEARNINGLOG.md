**week-1
======================================
 RUNNING BUGGY (Unsynchronized)
======================================
time ./buggy_test
Final counter value: 189744
Expected counter value: 800000
0.00user 0.01system 0:00.00elapsed 177%CPU (0avgtext+0avgdata 3768maxresident)k
0inputs+0outputs (0major+167minor)pagefaults 0swaps

======================================
RUNNING MUTEX (OS-Level Lock)
======================================
time ./mutex_test
Final counter value: 800000
Expected counter value: 800000
0.03user 0.00system 0:01.01elapsed 205%CPU (0avgtext+0avgdata 3900maxresident)k
0inputs+0outputs (0major+171minor)pagefaults 0swaps

======================================
 RUNNING ATOMIC (Hardware-Level Lock)
======================================
time ./atomic_test
Final counter value: 800000
Expected counter value: 800000
0.03user 0.00system 0:00.01elapsed 253%CPU (0avgtext+0avgdata 3924maxresident)k
0inputs+0outputs (0major+168minor)pagefaults 0swaps

**Key Takeaway:** The OS-level Mutex spent significant time in the kernel (`0.01s sys` time) putting threads to sleep. The Hardware-level Atomic instruction eliminated kernel overhead entirely (`0.00s sys` time), allowing the threads to fully utilize multiple CPU cores concurrently.