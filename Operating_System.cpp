/**
	Meredith Raab 
	Operating Systems Fall 2014
	Lab 3 - Banker's Algorithm
	Due November 13th 11:55pm
**/
#include <fstream>
#include "Operating_System.h"

Operating_System::Operating_System(string file){
	//open the file
	cycles = 0;

	ifstream input_file;
	input_file.open(file);

	if(input_file.is_open()){ //check if the file is open
		int amountOfEachResource[10]; // limit to 10 number of resource units 

		int numberOfTasks, numberOfResourceTypes;

		//first line of the file
		input_file >> numberOfTasks >> numberOfResourceTypes;

		//grab the number of each resource
		if(numberOfResourceTypes <=10){
			for(int i=0; i< numberOfResourceTypes; i++){
				input_file >> amountOfEachResource[i];
			}			
		}

		while(input_file.good()){
			Instruction *i = new Instruction;
			input_file >> i->command;
			input_file >> i->task_number;
			input_file >> i->delay;
			input_file >> i->resource_type;
			input_file >> i->arg5;

			allInstructions.push_back(i);
		}
	}
	else{
		cout << "Failed to Open File!";
	}
	input_file.close();
}

void Operating_System::Initiate(int task_number, int resource_type, int initial_claim){
	Task *t = new Task(task_number, initial_claim, cycles);
	//resource type and initial claim are important for bankers 
	tasks.push_back(t); //make sure tasks are in order -- needs to be pushed in order
						//so tasks[0] represents task 1, task[1] = task 2, etc. 
	increaseWaitingTimeOfAllWaitingTasks(1);
	cycles+=1;
}

void Operating_System::Request(int task_number, int delay, int resource_type, int number_requested){
	tasks[task_number-1]->setRunning();
	resources[resource_type-1]-=number_requested; 
	tasks[task_number-1]->addResources(resource_type, number_requested);
	increaseWaitingTimeOfAllWaitingTasks(1+delay);
	cycles+=1+delay;

}

void Operating_System::Release(int task_number, int delay, int resource_type, int number_released){
	tasks[task_number-1]->releaseResources(resource_type, number_released);
	resources[resource_type-1] += number_released;
	increaseWaitingTimeOfAllWaitingTasks(1+delay);
	cycles+=1+delay;
}

void Operating_System::Terminate(int task_number, int delay){
	tasks[task_number-1]->setTerminated();
	tasks[task_number-1]->setFinishCycle(cycles);
	increaseWaitingTimeOfAllWaitingTasks(delay);
	cycles+=delay;
}

bool Operating_System::canResourceRequestBeSatisfied(int resource_type, int number_requested){
	int correct_resource_type = resource_type-1; //subtract 1 to get the correct resource
	if(number_requested <= resources[correct_resource_type]){
		return true;
	}
	return false;
}

bool Operating_System::check_manager(int resource_type){
	Instruction *currentRequest = waiting_request_instructions_queue.front();
	if(resources[resource_type-1] > currentRequest->arg5){
		return true;
	}
	return false; 
}

void Operating_System::increaseWaitingTimeOfAllWaitingTasks(int number){
	for(int i=0; i<waiting_request_instructions_queue.size(); i++){
		Instruction *currentWaitingInstruction = waiting_request_instructions_queue.front();
		waiting_request_instructions_queue.pop();
		tasks[currentWaitingInstruction->task_number-1]->increaseWaitingTime(number);
		waiting_request_instructions_queue.push(currentWaitingInstruction);
	}
}

//deadlock = all non terminated tasks have outstanding requests that the manager cannot satisfy 
bool Operating_System::checkDeadlock(){

	bool deadlock = true;
	for(int i=0; i<waiting_request_instructions_queue.size(); i++){
		Instruction *currentRequest = waiting_request_instructions_queue.front();
		waiting_request_instructions_queue.pop();
		if(canResourceRequestBeSatisfied(currentRequest->resource_type, currentRequest->arg5)){
			deadlock = false;
		}
		waiting_request_instructions_queue.push(currentRequest);	
	}
	if(deadlock == false){
		return false;
	}
	return true;
}

//abort the first task in the waiting queue, add the resources back to the 
//resource manager
//do this until deadlock is resolved and resource manager has enough resources to satisfy
//the next request in the waiting queue
void Operating_System::handleDeadlock(){
	while(checkDeadlock()){
		Instruction *currentRequest = waiting_request_instructions_queue.front();
		resources[currentRequest->resource_type-1]+= currentRequest->arg5;
	}
}

void Operating_System::runOptimistic(){

	for(int i=0; i < allInstructions.size(); i++){
		Instruction *currentInstruction = allInstructions[i];
		if(currentInstruction->command == "initiate"){ //delay doesn't matter 
			Initiate(currentInstruction->task_number, currentInstruction->resource_type, currentInstruction->arg5);
		}
		else{ //delay now matters
			if(currentInstruction->command =="request"){
				if(canResourceRequestBeSatisfied(currentInstruction->resource_type, currentInstruction->arg5)){
					Request(currentInstruction->task_number, currentInstruction->delay, currentInstruction->resource_type, currentInstruction->arg5);
				}
				else{
					tasks[currentInstruction->task_number-1]->setWaiting();
					waiting_request_instructions_queue.push(currentInstruction);
				}
			}
			else if(currentInstruction->command == "release"){
				Release(currentInstruction->task_number, currentInstruction->delay, currentInstruction->resource_type, currentInstruction->arg5);
			}
			else if(currentInstruction->command == "terminate"){
				Terminate(currentInstruction->task_number, currentInstruction->delay);

			}
			//each cycle, check for deadlock 
			if(checkDeadlock()){
				handleDeadlock();
			}
		}
	}		
}

void Operating_System::runBankers(){

}
