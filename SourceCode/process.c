#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "common.h"
#include <time.h>
#include "stat.h"

int compare(void *data1, void *data2) {
	process *p1 = (process *)data1;
	process *p2 = (process *)data2;
    
	if (p1->arrivalTime < p2->arrivalTime) {
		return -1;
	} else if(p1->arrivalTime == p2->arrivalTime){
		return 0;
	} else {
		return 1;
	}
}

linked_list *generateProcesses(int n) {
	linked_list *process_list = createLinkedList();
	if (process_list == NULL) {
        fprintf(stderr, "Unable to create Linked List\n");
        return NULL;
    }

	// Initialize random number generator
	// srand((unsigned int)time(NULL));

	char pid = 'A';
	unsigned char priority;
	float arrivalTime, runTime;

   /* For debugging, intializes random number generator */
   int seed = 20;
   srand(seed);

    /* Create processes and add them to the list */
	int minRuntime = 1;
	int maxRuntime = 10;
	while (n--) {
		arrivalTime = rand() % 100;
		runTime = (rand() % (maxRuntime - minRuntime + 1)) + minRuntime;
		priority = (rand()%4)+1;
		process *p = createProcess(pid,arrivalTime,runTime,priority);
		if (p == NULL) {
            fprintf(stderr, "Unable to create process\n");
            continue;
        }
		addNode(process_list,p);
		pid++;
	}

	sort(process_list,compare);

	node *ptr = process_list->head;
	pid = 'A';
	while (ptr!=NULL) {
		((process *)ptr->data)->pid = pid;
		if(pid == 'Z') pid = 'a' - 1;
		pid++;
		ptr=ptr->next;
	}

	return process_list;
}

process *createProcess(char pid, int arrivalTime, int runTime, unsigned char priority) {
	process *proc = (process *) malloc(sizeof(process));
	proc->pid = pid;
	proc->arrivalTime = arrivalTime;
	proc->runTime = runTime;
	proc->priority = priority;
	return proc;
}
