#include "PrintManager.h"
#include "Process.h"
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>

PrintManager& PrintManager::getInstance() {
    static PrintManager instance;
    return instance;
}

void PrintManager::logInstruction(const Process& process, int coreId) {
    std::lock_guard<std::mutex> lock(printMutex);

    std::string processName = process.getName();
    int idx = process.getCurrentInstructionIndex();
    const auto& instructions = process.getInstructions();

    std::string commandText = "No Instruction";
    if (idx >= 0 && idx < instructions.size()) {
        commandText = instructions[idx];
    }

    // Capture time
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    auto duration = now.time_since_epoch();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000;

    std::tm timeInfo;
#if defined(_MSC_VER)
    localtime_s(&timeInfo, &time_t_now);
#else
    localtime_r(&time_t_now, &timeInfo);
#endif

    std::stringstream ts;
    ts << std::put_time(&timeInfo, "(%m/%d/%Y %I:%M:%S")
        << "." << std::setfill('0') << std::setw(3) << ms.count()
        << std::put_time(&timeInfo, " %p)");

    std::stringstream logLine;
    logLine << ts.str() << " Core " << coreId << ": " << commandText;

    memoryLogs[processName].push_back(logLine.str());
}

void PrintManager::flushToFiles() {
    std::lock_guard<std::mutex> lock(printMutex);

    if (memoryLogs.empty()) {
        std::cout << "No logs found in memory to print.\n";
        return;
    }

    std::cout << "Dumping logs to files... Please wait.\n";

    for (const auto& [processName, lines] : memoryLogs) {
        std::string filename = processName + ".txt";
        std::ofstream file(filename, std::ios::trunc); // Overwrites fresh on manual trigger

        if (file.is_open()) {
            file << "Process_name: " << processName << "\n";
            file << "Logs:\n";

            for (const auto& line : lines) {
                file << line << "\n";
            }
            file.close();
        }
    }

    std::cout << "Successfully generated text files for " << memoryLogs.size() << " processes.\n";
}