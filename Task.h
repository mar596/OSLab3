/**
	Meredith Raab 
	Operating Systems Fall 2014
	Lab 3 - Banker's Algorithm
	Due November 13th 11:55pm
**/

#ifndef __Task__
#define __Task__

#include <iostream>
#include <unordered_map>

using namespace std;

class Task{
public:
	//constructor
	Task(int tn, int c, int s) : task_number(tn), initial_claim(c), task_state(INITIALIZED), start(s), finish(-1), timeSpentWaiting(0){}

	void addResources(int resource_type, int number_requested){resource_map[resource_type] += number_requested;}
	void releaseResources(int resource_type, int number_requested){resource_map[resource_type] -= number_requested;}

	void setRunning(){task_state = RUNNING;}
	void setTerminated(){task_state = TERMINATED;}
	void setWaiting(){task_state = WAITING;}

	void increaseWaitingTime(int n){timeSpentWaiting+=n;}
	void setFinishCycle(int cycle){finish = cycle;}

 	//getters
	int getInitialClaim() const{return initial_claim;}
	int getTaskNumber() const{return task_number;}
	int getTimeSpentWaiting() const{return timeSpentWaiting;}
	int getTimeNeededToFinish() const{return (finish == -1 ? -1 : finish-start);}
	int getPercentageOfTimeSpentWaiting() const {return timeSpentWaiting / (finish - start);}

	bool isInitialized(){return task_state == INITIALIZED;}
	bool isRunning(){return task_state == RUNNING;}
	bool isTerminated(){return task_state == TERMINATED;}
	bool isWaiting(){return task_state == WAITING;}

private:

	enum TaskState{
 		INITIALIZED, 
 		WAITING,     
		RUNNING,  
 		TERMINATED   
 	};

	int timeSpentWaiting;
	int task_number;
	int initial_claim; 
	int start, finish;

	TaskState task_state; 
	unordered_map<int,int>resource_map; // stores the resource as the key and how many of that resource as the value  	
};

#endif 