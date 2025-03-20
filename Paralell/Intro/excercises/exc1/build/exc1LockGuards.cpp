#include <thread>
#include <memory>
#include <mutex>
#include <iostream>
#include <future>


void doSomething(std::mutex& mtxA, std::mutex& mtxB) {
    std::cout << "Hello from thread " << std::this_thread::get_id() << std::endl;
}

void threadFunc(std::mutex& mtxA, std::mutex& mtxB) {
    std::lock_guard<std::mutex> lockA(mtxA);  // Lock mtxA
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate work, increasing chance of deadlock
    std::lock_guard<std::mutex> lockB(mtxB);  // Lock mtxB
    doSomething(mtxA, mtxB); // Use both mutexes in the function
}


// notice order of mutexes
void deadLocked(std::mutex& mtxA, std::mutex& mtxB) {
    std::cout << "thread a" << std::endl;
    std::thread a(threadFunc, std::ref(mtxB), std::ref(mtxA));

    std::cout << "thread b" << std::endl;
    std::thread b(threadFunc, std::ref(mtxA), std::ref(mtxB));
    
    // Wait for both threads to finish, which will never happen due to deadlock
    a.join();
    b.join();
}

void solved(std::mutex& mtxA, std::mutex& mtxB) {
    std::cout << "thread a" << std::endl;
    std::thread a(threadFunc, std::ref(mtxB), std::ref(mtxA)); // Lock order reversed

    std::cout << "thread b" << std::endl;
    std::thread b(threadFunc, std::ref(mtxB), std::ref(mtxA)); // Lock order reversed


    std::cout<<"solve : wairing to finish"<<std::endl;
    // Wait for both threads to finish
    a.join();
    b.join();
}

void deadLockedWithTimeout(std::mutex& mtxA, std::mutex& mtxB) {
    std::cout << "Trying to execute with timeout..." << std::endl;

    // Use async to run the deadlock function and specify a timeout
    std::future<void> result = std::async(std::launch::async, [&]() {
        deadLocked(mtxA, mtxB); // This will deadlock
    });

    // Wait for 5 seconds and check if the thread is done
    if (result.wait_for(std::chrono::seconds(5)) == std::future_status::timeout) {
        std::cout << "Timed out after 5 seconds!" << std::endl;
        // Optionally, you could add logic to abort the thread here
    } else {
        std::cout << "Deadlock function completed successfully." << std::endl;
    }
}
int main() {
    
    std::cout<<"init"<<std::endl;

    std::mutex mtxA, mtxB;

    solved(mtxA, mtxB);

    // Now, test with deadlock and timeout
    deadLockedWithTimeout(mtxA, mtxB);



    return 0;
}