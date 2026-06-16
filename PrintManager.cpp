#include "PrintManager.h"
#include "Process.h"
#include "Instruction.h"
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>


PrintManager::PrintManager() : isEnabled(true) {}

PrintManager& PrintManager::getInstance() {
    static PrintManager instance;
    return instance;
}

void PrintManager::setEnabled(bool status) {
    isEnabled = status;
}
// TODO: will revise core parameter later
void PrintManager::logInstruction(const Process& process,  int coreId) {
    if (!isEnabled) return;

    // Guard the file pipeline across the 4 active core threads
    std::lock_guard<std::mutex> lock(printMutex);

    // Extract strings cleanly from our refined object parameters
    std::string processName = process.getName();
    std::string commandText = instruction.getCommandText();

    // Capture the exact moment of core CPU execution
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

    std::ofstream file(processName + ".txt", std::ios::app);
    if (file.is_open()) {
        file << ts.str() << " Core " << coreId << ": " << commandText << "\n";
        file.close();
    }
}