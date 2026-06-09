#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <chrono>
#include <cassert>
#include <cstdint>
 
using namespace std;
using namespace std::chrono;
 
static const int NUM_THREADS    = 8;
static const int OPS_PER_THREAD = 100'000;
static const int EXPECTED       = NUM_THREADS * OPS_PER_THREAD;
 

//  The Race Condition

//
// The most fundamental concurrency bug. Two threads read-modify-write the same
// memory location without coordination.
//
// What happens at the CPU level:
//   Thread A reads counter (value = 5)
//   Thread B reads counter (value = 5)  <-- context switch happens here
//   Thread A writes counter = 6
//   Thread B writes counter = 6         <-- B's increment is LOST
//
// This is not a bug in the compiler or OS. It is the correct behaviour of
// concurrent memory access without synchronisation. The hardware does exactly
// what it's told — two threads told it to write 6, so it wrote 6 twice.
//
// LEARNING: A race condition is not "sometimes" a bug. It is always undefined
// behaviour in C++. The program may appear correct 99% of the time and fail
// on a production server under high load. That 1% is catastrophic.
 
// Shared counter variable. All threads will read-modify-write this variable.
long long counter = 0;

// Worker thread function. Each thread will increment the counter this many times.
void worker() {
    for (int i = 0; i < OPS_PER_THREAD; i++) {
        
        counter++;
    }
}

int main() {
    vector<thread> threads;
 
    // Start the worker threads.
    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back(worker);
    }
 
    // Wait for all threads to finish.
    for (auto& t : threads) {
        t.join();
    }
 
    // Check the final value of the counter.
    cout << "Final counter value: " << counter << endl;
    cout << "Expected counter value: " << EXPECTED << endl;
    //assert(counter == EXPECTED);
 
    return 0;
}
