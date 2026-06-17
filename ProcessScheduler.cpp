#include "processScheduler.h"

ProcessScheduler::ProcessScheduler(int numCores) : numCores(numCores) {}

void ProcessScheduler::addProcess(std::shared_ptr<Process> process) {
    std::lock_guard<std::mutex> lock(mtx);
    readyQueue.push(process);
    cv.notify_one();
}

void ProcessScheduler::run() {
    std::vector<std::thread> workers;

    for (int coreId = 0; coreId < numCores; coreId++) {
        workers.emplace_back([this, coreId]() {
            while (true) {
                std::shared_ptr<Process> process;

                {
                    std::unique_lock<std::mutex> lock(mtx);
                    cv.wait(lock, [this] {
                        return !readyQueue.empty() || stop;
                        });

                    if (stop && readyQueue.empty()) break;

                    process = readyQueue.front();
                    readyQueue.pop();
                }

                process->setCore(coreId);

				// insert process execution logic 

                {
                    std::lock_guard<std::mutex> lock(mtx);
                    finishedProcesses.push_back(process);
                }
            }
            });
    }

    for (auto& t : workers)
        t.join();
}

void ProcessScheduler::shutdown() {
    std::lock_guard<std::mutex> lock(mtx);
    stop = true;
    cv.notify_all();
}