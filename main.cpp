/**
	Meredith Raab 
	Operating Systems Fall 2014
	Lab 3 - Banker's Algorithm
	Due November 13th 11:55pm
**/

//FIX VARIABLE NAMES TO BE MORE CONSISTENT

#include "Operating_System.h" 

using namespace std;

int main(int argc, char* argv[]){

	if(argc != 2){ // the second argument on command line is the input file 
		cout << "Incorrect Input.";
		exit(1);
	}

	Operating_System os(argv[1]);
}