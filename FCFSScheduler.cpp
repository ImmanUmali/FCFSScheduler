#include <iostream>
#include "FCFSScheduler.h"
#include "Process.h"

int main() {
    FCFSScheduler scheduler(4); // 4 cores

    // Create some test processes and add them to the scheduler
    for (int i = 1; i <= 5; i++) {
        auto process = std::make_shared<Process>("process" + std::to_string(i));
        scheduler.addProcess(process);
        std::cout << "Added: " << process->name << "\n";
    }

    // Run the scheduler (blocks until all processes are done)
    std::thread runThread([&scheduler]() {
        scheduler.run();
    });

    // Give it time to finish then shut down
    std::this_thread::sleep_for(std::chrono::seconds(2));
    scheduler.shutdown();

    runThread.join();

    // Print finished processes
    std::cout << "\nFinished processes:\n";
    for (auto& p : scheduler.finishedProcesses) {
        std::cout << "  " << p->name << "\n";
    }

    return 0;
}
