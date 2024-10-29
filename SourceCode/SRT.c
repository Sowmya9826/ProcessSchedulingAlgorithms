#include <stdio.h>
#include <stdlib.h>
#include "stat.h"
#include "common.h"

process_stat *generateProcess(process *proc);

// printQueueSRT is a function to print the contents of the queue during srt
void printQueueSRT(queue *q)
{
  if (q->head != NULL)
  {
    node *n = q->head;
    printf("Queue Contains:\n");
    while(n != NULL)
    {
      process_stat *ps = n->data;
      process *p = ps->proc;
      printf("Process Id %c Remaining Time %f\n",p->pid,(p->runTime - ps->runTime));
      n = n->next;
    }
  }
  return;
}
//  CRT function helps to compare the completion time of processess during sorting  
int Crt(void *data1, void *data2)
{
  process_stat *ps1 = (process_stat *) data1;
	process_stat *ps2 = (process_stat *) data2;
	if(((((process *)ps1->proc)->runTime) - (ps1->runTime)) < ((((process *)ps2->proc)->runTime) - (ps2->runTime))) {
		return -1;
	} else {
		return 1;
	}
}



// Implementation of shortest remaining time to completion preemptive 
average_stats shortestRemainingTimePreemptive(linked_list *processes)
{
  int currentTime = 0; 

  // creation of a queue of processes
  queue *processQueue = createQueue();
  // creation of linked list for managaing the order of processes in preemeption
  linked_list *ll = createLinkedList();

  node *processPointer = processes->head;
  if(processes->head == NULL) {
		fprintf(stderr,"No Process to schedule\n");
	}

  //runs until  time quanta is less than 100 or process queue is not empty 
  process_stat *scheduledProcess = NULL;
	printf("\n\n\n==================================================================================================================================\n");
  printf("\n\n\n==================================================================================================================================\n");

  printf("\nSHORTEST REMAINING TIME PREEMPTIVE::\n");
  printf("Order of Processes: ");
  while(currentTime<100 || scheduledProcess!=NULL )
  {
 
    if(scheduledProcess!=NULL)
    {
      enqueue(processQueue,scheduledProcess);
      scheduledProcess = NULL;
    }
    //check for incoming new process and enqueue it in the queue
		if(processPointer != NULL) {
			process *newProcess = (process *)(processPointer->data);
			while(processPointer !=NULL && newProcess->arrivalTime <= currentTime) {
				enqueue(processQueue,generateProcess(newProcess));
				processPointer = processPointer->next;
				if(processPointer!=NULL)
        {
					newProcess = (process *)(processPointer->data);
        }
			}
      // Sorting all the processes that arrived in the queue depending on their remaining running time
      sort(processQueue,Crt);
    }

    //if scheduled process is null ,it checks process queue and it schedules  
		if(scheduledProcess == NULL && processQueue->size > 0) {
			scheduledProcess = (process_stat *) dequeue(processQueue);

      // process is removed from the queue if it hasn't started befOre time quanta 100 and next process in queue is executed.
      while(currentTime>=100 && scheduledProcess->startTime == -1)
      {
        scheduledProcess = (process_stat *) dequeue(processQueue);
      }
		}
    if(scheduledProcess != NULL) {
  			process *proc = scheduledProcess->proc;

  			// running process is added 
  			printf("%c",proc->pid);

  			//update the current processes stat
  			if(scheduledProcess->startTime == -1) {
  				scheduledProcess->startTime = currentTime;
  			}

  			scheduledProcess->runTime++;

        if(scheduledProcess->runTime >= proc->runTime) {
          scheduledProcess->endTime = currentTime;
          addNode(ll,scheduledProcess);
          scheduledProcess = NULL;
        }
      }else {
    			printf("_");
    	}
  	
  		currentTime++;
  }
  //Print Process Stat
  return printPolicyStat(ll);
  
}