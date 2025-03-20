#include <thread>
#include <mutex>
#include <iostream>
#include <future>
#include <chrono>


std::mutex mutex1;
std::mutex mutex2;
// Function that simulates the deadlock scenario
void doSomethingWithDeadlock(bool lockOrder) {
    if (lockOrder) {  // Thread 1 locks mutex1 then mutex2
        std::cout << "Thread 1: Trying to lock mutex1..." << std::endl;
        mutex1.lock();
        std::cout << "Thread 1: Locked mutex1." << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Simulate work

        std::cout << "Thread 1: Trying to lock mutex2..." << std::endl;
        mutex2.lock();
        std::cout << "Thread 1: Locked mutex2." << std::endl;

    } else {  // Thread 2 locks mutex2 then mutex1
        std::cout << "Thread 2: Trying to lock mutex2..." << std::endl;
        mutex2.lock();
        std::cout << "Thread 2: Locked mutex2." << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Simulate work

        std::cout << "Thread 2: Trying to lock mutex1..." << std::endl;
        mutex1.lock();
        std::cout << "Thread 2: Locked mutex1." << std::endl;
    }

    // Critical section
    std::cout << "Hello from thread " << std::this_thread::get_id() << std::endl;

    // Unlock the mutexes in reverse order of locking
    if (lockOrder) {
        mutex2.unlock();
        mutex1.unlock();
    } else {
        mutex1.unlock();
        mutex2.unlock();
    }
}




// Function to handle the deadlock with timeout and exception handling
void deadLockedWithTimeout(bool lockOrder) {
    std::cout << "Trying to execute with timeout..." << std::endl;

    // Use async to run the deadlock function and specify a timeout
    std::future<void> result = std::async(std::launch::async, [&]() {
        doSomethingWithDeadlock(lockOrder); // This will deadlock
    });

    // Wait for 5 seconds and check if the thread is done
    try {
        if (result.wait_for(std::chrono::seconds(5)) == std::future_status::timeout) {
            throw std::runtime_error("Timed out after 5 seconds!");
        } else {
            std::cout << "Deadlock function completed successfully." << std::endl;
        }
    } catch (const std::runtime_error& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;

        std::cout << "Thread_ " << std::this_thread::get_id() << std::endl;

        return;
    }
}


// Function that uses std::scoped_lock to prevent deadlock
void scopedLockWithTimeout(bool lockOrder) {
    if (lockOrder) {  // Thread 1 locks mutex1 then mutex2
        std::cout << "Thread 1: Trying to lock mutex1 and mutex2..." << std::endl;
        std::scoped_lock lock(mutex1, mutex2); // Lock both mutexes in a deadlock-free manner
        std::cout << "Thread 1: Locked mutex1 and mutex2." << std::endl;

    } else {  // Thread 2 locks mutex2 then mutex1
        std::cout << "Thread 2: Trying to lock mutex2 and mutex1..." << std::endl;
        std::scoped_lock lock(mutex2, mutex1); // Lock both mutexes in a deadlock-free manner
        std::cout << "Thread 2: Locked mutex2 and mutex1." << std::endl;
    }

    // Critical section
    std::cout << "Hello from thread " << std::this_thread::get_id() << std::endl;

    // Mutexes will be automatically unlocked when going out of scope (no need to manually unlock)
}


int main() {
    std::cout << "init" << std::endl;

    // Create two mutexes
    std::mutex mtxA, mtxB;

    // Start the threads
    std::cout << "Starting Thread 1 (locks mutex1 -> mutex2)" << std::endl;
    std::thread t1(deadLockedWithTimeout, true); // Thread 1 locks mutex1 -> mutex2

    std::cout << "Starting Thread 2 (locks mutex2 -> mutex1)" << std::endl;
    std::thread t2(deadLockedWithTimeout, false); // Thread 2 locks mutex2 -> mutex1

    // Wait for both threads to finish
    t1.join();
    t2.join();


    // Add a separation between deadlock and scoped lock test
    std::cout << "\n--- Now testing the deadlock-free solution using std::scoped_lock ---\n" << std::endl;

    // Scoped Lock Solution
    std::cout << "Starting Thread 1 (locks mutex1 -> mutex2) with scoped_lock" << std::endl;
    std::thread t3(scopedLockWithTimeout, true); // Thread 1 locks mutex1 -> mutex2

    std::cout << "Starting Thread 2 (locks mutex2 -> mutex1) with scoped_lock" << std::endl;
    std::thread t4(scopedLockWithTimeout, false); // Thread 2 locks mutex2 -> mutex1

    // Wait for both threads to finish
    t3.join();
    t4.join();

    return 0;
    

    return 0;
}
