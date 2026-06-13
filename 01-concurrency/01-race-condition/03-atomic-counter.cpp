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


atomic<long long>counter(0);


void good_worker() {
    for (int i = 0; i < OPS_PER_THREAD; i++) {
          counter++;
    }
}

int main() {
    vector<thread> threads;

    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back(good_worker);
    }

    for (auto& t : threads) {
        t.join();
    }
 
    // Check the final value of the counter.
    cout << "Final counter value: " << counter.load() << endl;
    cout << "Expected counter value: " << EXPECTED << endl;
    
    return 0;
}

/*1. The Core Concept: Hardware over OS
AAn OS-level mutex is s
low because it involves putting threads to sleep and waking them up (Context Switching).
An atomic operation is fundamentally different. It guarantees that a 
read-modify-write operation happens in a single, unbreakable hardware step. The CPU core literally 
locks the memory bus for a fraction of a nanosecond, does the math, 
and unlocks it. The OS kernel never even knows it happened.

2-Essential builtins
std::atomic<T> (The Wrapper):
This is a template class. You wrap your standard data types inside it to make them safe.
Example: std::atomic<long long> counter = 0;
Example: std::atomic<bool> is_running = true;
fetch_add() and fetch_sub():
These are the most critical functions for counters. They add or subtract a value in one unbreakable CPU cycle.
Example: counter.fetch_add(1);

Magic shortcut: The C++ standard library overloads the ++ and -- operators for atomic variables. 
So, typing counter++; on a std::atomic variable automatically translates to a hardware-safe fetch_add!

.load() and .store():
Standard reads and writes (counter = 5 or cout << counter) can sometimes be optimized away or reordered by the compiler.
 Using .load() and .store() forces the compiler to read the absolute latest value directly from RAM, not from a stale CPU cache.

Example: long long current_val = counter.load();

Example: counter.store(0);

compare_exchange_strong() (Advanced):
Often called CAS (Compare-And-Swap), this is the foundation of all advanced "lock-free" data structures. 
It says: "Update this variable to X, but ONLY if its current value is Y. If someone else changed it 
to Z while I wasn't looking, fail and let me try again." 
*/