#include <stdio.h>
#include <stdlib.h>
#include "stat.h"
#include "common.h"

process_stat *generateProcess(process *proc);
int comparePriority(void *data1, void *data2);
void printqueue(queue *q);


// Implementation of Non Preemptive HPF with FCFS 
average_stats highestPriorityFirstNonPreemptive(linked_list *processes)
{
  int currentTime = 0; // quanta 
  // create a queue of processes.
  queue * processQueue1 = createQueue();
  queue * processQueue2 = createQueue();
  queue * processQueue3 = createQueue();
  queue * processQueue4 = createQueue();
  
  
  // create linked list to store results of statistics
  linked_list * linkedlist1 = createLinkedList();
  linked_list * linkedlist2 = createLinkedList();
  linked_list * linkedlist3 = createLinkedList();
  linked_list * linkedlist4 = createLinkedList();
  
  
  node * processPointer = processes->head;
  if(processes->head == NULL) {
		fprintf(stderr,"There is no Process to schedule\n");
	}
	printf("\n\n\n==================================================================================================================================\n");
  printf("\n\n\n==================================================================================================================================\n");

  printf("\nHIGHEST PRIORITY FIRST NON PREEMPTIVE::\n");
  printf("Order of Processes: ");
  
  
 //check time quanta is less than 100 or the process queue is empty...
 
 
  process_stat * scheduledProcess = NULL;
  while(currentTime < 100 || scheduledProcess != NULL )
  {
         //check for incoming new process and do enqueue based on priority.
		 
		if(processPointer != NULL) {
			process * newProcess = (process *)(processPointer->data);
			
		//Continue verifying whether the process pointer is not null and if the arrival time of the new process is less than or equal to the current time t then processes are enqueued into the appropriate priority queue 
			while(processPointer !=NULL && newProcess->arrivalTime <= currentTime) {
				
				
        if(newProcess->priority == 1) 
          enqueue(processQueue1,generateProcess(newProcess));
        if(newProcess->priority == 2) 
          enqueue(processQueue2,generateProcess(newProcess));
        if(newProcess->priority == 3) 
          enqueue(processQueue3,generateProcess(newProcess));
        if(newProcess->priority == 4) 
          enqueue(processQueue3,generateProcess(newProcess));
		
		
				//sort(processQueue,comparePriority);
				processPointer = processPointer->next;
				if(processPointer!=NULL)
        {
					newProcess = (process *)(processPointer->data);
        }
			}
      // sorting arrived processes based on priority.
      //sort(processQueue,comparePriority);
    }
    
		if(scheduledProcess == NULL) {
			
			//dequeue processes on their sizes
      if (processQueue1->size > 0) 
        scheduledProcess = (process_stat *) dequeue(processQueue1);
      else if (processQueue2->size > 0) 
        scheduledProcess = (process_stat *) dequeue(processQueue2);
      else if (processQueue3->size > 0) 
        scheduledProcess = (process_stat *) dequeue(processQueue3);
      else if (processQueue4->size > 0) 
        scheduledProcess = (process_stat *) dequeue(processQueue4);
	  
	  
        // If the process hasn't commenced by quantum 100, eliminate the process from the queue and proceed with the next process in line for execution
     
	      //if the time > = 100 an startime -1,null the scheduled process.
	 
      if (currentTime>=100 && scheduledProcess->startTime == -1){
        //free(scheduledProcess);
        scheduledProcess = NULL;
        continue;
      }
    }


    if(scheduledProcess != NULL) {
  			process * proc = scheduledProcess->proc;

  			//add  the currently running process to the time chart
  			printf("%c",proc->pid);

  			//updating the  current processes stat
  			if(scheduledProcess->startTime == -1) {
  				scheduledProcess->startTime = currentTime;
  			}

  			scheduledProcess->runTime++;

    
        if(scheduledProcess->runTime >= proc->runTime) {
          scheduledProcess->endTime = currentTime;
		  
		        //add nodes to the linked list based on their priority.
		  
          if(scheduledProcess->proc->priority == 1) 
            addNode(linkedlist1,scheduledProcess);
          else if(scheduledProcess->proc->priority == 2) 
            addNode(linkedlist2,scheduledProcess);
          else if(scheduledProcess->proc->priority == 3) 
            addNode(linkedlist3,scheduledProcess);
          else if(scheduledProcess->proc->priority == 4) 
            addNode(linkedlist4,scheduledProcess);
          //addNode(ll,scheduledProcess);
          scheduledProcess = NULL;
          //free(scheduledProcess);
        }
      }else {
    			printf("_");
  		}
  		//increase the time...
  		currentTime++;
    }
    //Print Process Stat
    average_stats avg1,avg2,avg3,avg4,avg;
    printf("\nFor the  Priority Queue 1");
    avg1 = printPolicyStat(linkedlist1);
    printf("\nFor the  Priority Queue 2");
    avg2 = printPolicyStat(linkedlist2);
    printf("\nFor the  Priority Queue 3");
    avg3 = printPolicyStat(linkedlist3);
    printf("\nFor  the Priority Queue 4");
    avg4 = printPolicyStat(linkedlist4);

//calcultaing the mean response time ,wait time,turnaround time and throughput.
    avg.avg_response_time = (avg1.avg_response_time + avg2.avg_response_time + avg3.avg_response_time + avg4.avg_response_time)/4 ;
    avg.avg_wait_time = (avg1.avg_wait_time + avg2.avg_wait_time + avg3.avg_wait_time + avg4.avg_wait_time)/4 ;
    avg.avg_turnaround = (avg1.avg_turnaround + avg2.avg_turnaround + avg3.avg_turnaround + avg4.avg_turnaround)/4 ;
    avg.avg_throughput = (avg1.avg_throughput + avg2.avg_throughput + avg3.avg_throughput + avg4.avg_throughput) ;


    printf("\nTHE AVERAGE TIME FOR HIGH PRIORITY FIRST NON PREEMPTIVE ALL QUEUES::\n");
    printf("|------------------------------------------------------------|\n");
    printf("| Statistics                  | Average                 |\n");
    printf("|---------------------------|--------------------------------|\n");
    printf("| Response-Time     | %.1f                           |\n", avg.avg_response_time);
    printf("| Wait-Time         | %.1f                           |\n", avg.avg_wait_time);
    printf("| Turn-Around-Time  | %.1f                           |\n", avg.avg_turnaround);
    printf("|------------------------------------------------------------|\n");

    return avg;
}


