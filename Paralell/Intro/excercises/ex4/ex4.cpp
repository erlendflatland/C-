#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <chrono>

std::shared_mutex shared_mtx;  // Shared mutex used by both implementations
int shared_data = 0;

// Faulty Implementation (Incorrect use of std::shared_mutex)
void faulty_reader(int id) {
    while (true) {
        std::unique_lock lock(shared_mtx);  // Misusing unique_lock for reading
        std::cout << "[Faulty] Reader " << id << " reads shared_data = " << shared_data << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void faulty_writer() {
    while (true) {
        std::unique_lock lock(shared_mtx);  // Correctly locking for writing
        ++shared_data;
        std::cout << "[Faulty] Writer updated shared_data to " << shared_data << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));

    }
}

// Correct Implementation (Using std::shared_lock for readers)
void correct_reader(int id) {
    while (true) {
        std::shared_lock lock(shared_mtx);  // Properly using shared_lock for reading
        std::cout << "[Correct] Reader " << id << " reads shared_data = " << shared_data << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void correct_writer() {
    while (true) {
        std::unique_lock lock(shared_mtx);  // Correctly locking for writing
        ++shared_data;
        std::cout << "[Correct] Writer updated shared_data to " << shared_data << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));

    }
}

int main() {
    // Run the faulty implementation
    std::cout << "Running Faulty Implementation...\n";
    std::thread faulty_writer_thread(faulty_writer);
    std::thread faulty_reader1(faulty_reader, 1);
    std::thread faulty_reader2(faulty_reader, 2);

    std::this_thread::sleep_for(std::chrono::seconds(10));  // Let the faulty implementation run for a while

    faulty_writer_thread.detach();
    faulty_reader1.detach();
    faulty_reader2.detach();

    // Run the correct implementation
    std::cout << "\nRunning Correct Implementation...\n";
    std::thread correct_writer_thread(correct_writer);
    std::thread correct_reader1(correct_reader, 1);
    std::thread correct_reader2(correct_reader, 2);

    correct_writer_thread.join();
    correct_reader1.join();
    correct_reader2.join();

    return 0;
}
