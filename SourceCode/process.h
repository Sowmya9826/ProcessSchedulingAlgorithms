#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "common.h"

// Structure to represent a process
struct process_s {
    char pid;      // Process ID
    int arrivalTime;        // Arrival time of the process
    int runTime;            // Run time of the process
    unsigned char priority; // Priority of the process
};

typedef struct process_s process;

// Function to create a new process
process *createProcess(char pid, int arrivalTime, int runTime, unsigned char priority);

// Function to get a copy of a process
process *getCopyofProcess(process *proc);

// Function to generate a list of processes
linked_list *generateProcesses(int n);

#endif // _PROCESS_H_
