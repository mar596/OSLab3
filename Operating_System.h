/**
	Meredith Raab 
	Operating Systems Fall 2014
	Lab 3 - Banker's Algorithm
	Due November 13th 11:55pm
**/

#ifndef __Operating_System__
#define __Operating_System__

#include <iostream>
#include <queue>
#include <vector>
#include "Task.h"

using namespace std;

class Operating_System{
public:
	Operating_System(string file);

	void runOptimistic();

	void runBankers();

private:
	struct Instruction{
		string command;
		int task_number; 
		int delay;
		int resource_type;
		int arg5;
	};

	vector<Instruction*> allInstructions;
	queue<Instruction*> waiting_request_instructions_queue;
	vector<int> resources;
	vector<Task*> tasks; //vector of pointers to tasks 
	int cycles;

	void Initiate(int task_number, int resource_type, int initial_claim);
	void Request(int task_number, int delay, int resource_type, int number_requested);
	void Release(int task_number, int delay, int resource_type, int number_released);
	void Terminate(int task_number, int delay);
	void increaseWaitingTimeOfAllWaitingTasks(int number);
	bool canResourceRequestBeSatisfied(int resource_type, int number_requested);
	void incrementWaitingTimeOfTask();
	bool check_manager(int resource_type);
	bool checkDeadlock();
	void handleDeadlock();

	void printOutput();

};

#endif