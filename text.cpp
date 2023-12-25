#include <cstdio>
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

std::atomic<bool> stopWatchActive(true);

void stopwatch() {
    auto startTime = std::chrono::steady_clock::now();

    while (stopWatchActive) {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();

        std::cout << "Elapsed time: " << elapsedTime << " seconds\n";

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    // Start stopwatch thread
    std::thread stopwatchThread(stopwatch);

    // Continue with the main program
    std::cout << "Main program is running...\n";
    char ch = getchar();
    // Sleep for 5 seconds as an example
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Stop the stopwatch
    stopWatchActive = false;

    // Wait for the stopwatch thread to finish
    stopwatchThread.join();

    std::cout << "Main program completed.\n";

    return 0;
}
