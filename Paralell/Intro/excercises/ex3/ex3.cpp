#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex mtx;

void thread1_tryLock() {
    for (int i = 0; i < 5; ++i) {
        printf("Thread 1: Attempting to lock the mutex. Attempt %d of 5.\n", i + 1);

        
        if (mtx.try_lock()) {
            std::cout << "Thread 1: Successfully locked the mutex.\n";
            mtx.unlock();
            return;
        } else {
            std::cout << "Thread 1: Mutex is locked by another thread. Retrying...\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    std::cout << "Thread 1: Failed to lock the mutex after 5 attempts.\n";
}

void thread1() {
    std::cout << "Thread 1: Attempting to lock mutex...\n";
    mtx.lock();  // Blocks until the mutex is available
    std::cout << "Thread 1: Successfully locked the mutex.\n";
    mtx.unlock();
}

void thread2() {
    mtx.lock();
    std::cout << "Thread 2: Mutex locked. Holding it for 3 seconds...\n";
    std::this_thread::sleep_for(std::chrono::seconds(3));
    mtx.unlock();
    std::cout << "Thread 2: Mutex released.\n";
}

int main() {

    printf("trylock \n");

    {
        std::thread t2(thread2); // Start Thread 2 first
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Small delay to ensure Thread 2 locks the mutex
    
        std::thread t1(thread1_tryLock); // Now start Thread 1
    
        t1.join();
        t2.join();
    }

    printf("now w lock \n");
   
    {
        std::thread t2(thread2); // Start Thread 2 first
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Small delay to ensure Thread 2 locks the mutex
    
        std::thread t1(thread1); // Now start Thread 1
    
        t1.join();
        t2.join();
    }

    return 0;
}
