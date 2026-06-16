// Process Representation for no. 2
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <mutex>

class PrintManager {
private:
	PrintManager() = default;
	~PrintManager() = default;
	

	bool isEnabled = true; // enable creating txt files
	std::mutex printMutex;

public:
	static PrintManager& getInstance();

	void setEnabled(bool status);

	void logInstruction(const Process& process, const Instruction& instruction, int coreId);
};