#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>

// this is shared resources for the synchronization
std::mutex mtx;
std::condition_variable cv;
bool thread1Done = false;

// function for counting up from 1 to 20
void countUp() {
    // acquiring lock on the mutex
    std::unique_lock<std::mutex> lock(mtx);
    for (int i = 1; i <= 20; ++i) {
        std::cout << "Counting up for thread 1: " << i << std::endl;
    }
    // mark thread 1 as complete and notify thread 2
    thread1Done = true;
    cv.notify_one(); // signal for thread 2 that it can start
}

// function for counting down from 20 to 0
void countDown() {
    // acquire the lock and wait for the signal from thread 1
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [] { return thread1Done; }); // wait until thread 1 is done
    for (int i = 20; i >= 0; --i) {
        std::cout << "Counting down for thread 1: " << i << std::endl;
    }
}

int main() {
    // create and start thread 1 for counting up
    std::thread t1(countUp);

    // create and start thread 2 for counting dow
    std::thread t2(countDown);

    // waiting for both threads to finish
    t1.join(); // joining thread 1
    t2.join(); // joining thread 2

    return 0;
}



