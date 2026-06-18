#include "ProcessScheduler.h"
#include "PrintManager.h"
#include <chrono>
#include <iomanip>

ProcessScheduler::ProcessScheduler(int numCores) : numCores(numCores) {
    for (int i = 0; i < numCores; i++) {
        cpuCores.push_back(std::make_shared<Core>(i));
    }
}

void ProcessScheduler::addProcess(std::shared_ptr<Process> process) {
    std::lock_guard<std::mutex> lock(mtx);
    readyQueue.push(process);
    cv.notify_one();
}

void ProcessScheduler::start() {
    for (int coreId = 0; coreId < numCores; coreId++) {
        workers.emplace_back([this, coreId]() {
            auto myCore = cpuCores[coreId]; 

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
                    
                    process->setCore(coreId);
                    process->setState(RUNNING);
                    
                    myCore->assignProcess(process);
                }

                // Core processes instructions
                while (!process->isFinished() && !stop) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Simulated CPU burst

                    std::lock_guard<std::mutex> lock(mtx);

                    PrintManager::getInstance().logInstruction(*process, coreId);

                    myCore->stepExecution();
                }

                {
                    std::lock_guard<std::mutex> lock(mtx);
                    process->setState(FINISHED);
                    finishedProcesses.push_back(process);
                    myCore->releaseProcess(); 
                }
            }
        });
    }
}

void ProcessScheduler::shutdown() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        stop = true;
    }
    cv.notify_all();
    
    for (auto& t : workers) {
        if (t.joinable()) t.join();
    }
}

// screen -ls
void ProcessScheduler::printScreenLS() {
    std::lock_guard<std::mutex> lock(mtx);
    
    std::cout << "--------------------------------------------------------------\n";
    std::cout << "Running processes:\n";
    
    bool runningEmpty = true;
    for (const auto& core : cpuCores) {
        if (core->isBusy()) {
            runningEmpty = false;
            auto p = core->getCurrentProcess();
            std::cout << std::left  << std::setw(15) << p->getName()
                      << std::left  << std::setw(26) << ("(" + p->getDateTimeCreated() + ")")
                      << std::left  << std::setw(10) << ("Core: " + std::to_string(core->getId()))
                      << std::right << std::setw(5)  << p->getCurrentLine() 
                      << " / " << p->getTotalLines() << "\n";
        }
    }
    if (runningEmpty) std::cout << "(No running processes)\n";

    std::cout << "\nFinished processes:\n";
    if (finishedProcesses.empty()) {
        std::cout << "(No finished processes)\n";
    } else {
        for (const auto& p : finishedProcesses) {
            std::cout << std::left  << std::setw(15) << p->getName()
                      << std::left  << std::setw(26) << ("(" + p->getDateTimeCreated() + ")")
                      << std::left  << std::setw(10) << "Finished"
                      << std::right << std::setw(5)  << p->getTotalLines() 
                      << " / " << p->getTotalLines() << "\n";
        }
    }
    std::cout << "--------------------------------------------------------------\n";
}