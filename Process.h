// Process Representation for no. 2
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Instruction.h"


enum State
{
	NEW,
	RUNNING,
	WAITING,
	READY
};

class Process {
private:
	int pid;
	enum State state;
	std::string name;
	std::string dateTimeCreated; 
	int assignedCore;

	std::vector<Instruction> instruction_list;

public:
	Process(int pid, std::string name, std::string dateTimeCreated, std::vector<std::string> instructions)
		: pid(pid), name(name), dateTimeCreated(dateTimeCreated), assignedCore(-1),
		instruction_list(instructions),  state(NEW) {
	}

	//void printProcessInfo() {
	//	std::cout << name << " \t(" << dateTimeCreated << ") \tCore: " << assignedCore
	//		<< " " << instructionCount << " / " << instructions.size() << std::endl;
	//}

	int getPid() {
		return pid;
	}
	void setCore(int coreNo) {
		assignedCore = coreNo;
	}
};
