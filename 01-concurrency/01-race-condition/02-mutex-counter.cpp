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

long long counter = 0;
mutex mtx;

/* // Manual Locking
void bad_worker() {
    for (int i = 0; i < OPS_PER_THREAD; i++) {
        mtx.lock();
        counter++;
        // If an exception occurs here, the thread dies, the lock is never released, 
        // and the entire program deadlocks.
        mtx.unlock();
    }
}
*/

// The Industry Standard: RAII Locking
void good_worker() {
    for (int i = 0; i < OPS_PER_THREAD; i++) {
        // lock_guard automatically locks on creation and unlocks on destruction
        lock_guard<mutex> lock(mtx);
        counter++;
    }
}

int main() {
    vector<thread> threads;
 
    // Start the worker threads.
    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back(good_worker);
    }
 
    // Wait for all threads to finish.
    for (auto& t : threads) {
        t.join();
    }
 
    // Check the final value of the counter.
    cout << "Final counter value: " << counter << endl;
    cout << "Expected counter value: " << EXPECTED << endl;
    
    return 0;
}

//learning mutex is slow as it decides to hold a lock then make remainings which are trying to grab sleep 
//  then again waking up, it implies heavy context swithcing between threads