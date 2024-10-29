#include <stdio.h>
#include <stdlib.h>
#include "stat.h"
#include "common.h"

// Processes are generated, and their statistics are initialized
process_stat *generateProcess(process *proc) {
    process_stat *newProcessStat = (process_stat *) malloc(sizeof(process_stat));
    newProcessStat->proc = proc;
    newProcessStat->waitingTime = 0;
    newProcessStat->turnaroundTime = 0;
    newProcessStat->responseTime = 0;
    newProcessStat->runTime = 0;
    newProcessStat->startTime = -1;
    newProcessStat->endTime = -1;
    return newProcessStat;
}

//FCFS scheduling algorithm ensures that processes are executed in the order they arrive, without any preemption.
average_stats firstComeFirstServeNonPreemptive(linked_list *processes) {
    int currentTime = 0;

    // Establish the Process Queue
    queue *processQueue = (queue *)createQueue(); //to manage processes waiting to be executed.
    node *processPointer = processes->head;
    
    if (processes->head == NULL) {
        fprintf(stderr,"\nNo procedure for scheduling");
    }
    
    // Continue to monitor the currentTime quanta is below 100 or process queue remains empty
    process_stat *scheduledProcess = NULL;
    linked_list *ll = createLinkedList();
    
    printf("\n\n\n==================================================================================================================================\n");
    printf("\n\n\n==================================================================================================================================\n");

    printf("\nFIRST COME FIRST SERVE NON PREEMPTIVE::");
    printf("\nOrder of Processes: ");
    
    while (currentTime < 100 || scheduledProcess != NULL) { //loop runs until currentTime reaches 100 or there are no more scheduled processes.
        // check incoming new process and enqueue
        if (processPointer != NULL) {
            process * newProcess = (process *)(processPointer->data);
            if (newProcess->arrivalTime <= currentTime) { //ensures that the process is added to the ready queue when it arrives in the simulation.
                enqueue(processQueue, generateProcess(newProcess));
                processPointer = processPointer->next;
            }
        }

        // check process queue if there is no scheduled process now it will be added to ready queue
        if (scheduledProcess == NULL && processQueue->size > 0) {
            scheduledProcess = (process_stat *) dequeue(processQueue);
        }

        if (scheduledProcess != NULL) {
            process *proc = scheduledProcess->proc;

            //  add current running process to currentTime 
            printf("%c", proc->pid);

            // update current processes stat
            if (scheduledProcess->startTime == -1) {
                scheduledProcess->startTime = currentTime;
            }
            
            scheduledProcess->runTime++;
            
            // To check if process run currentTime is greater than the currently running process runtime
            if (scheduledProcess->runTime >= proc->runTime) {
                scheduledProcess->endTime = currentTime;
                
                // add scheduled process
                addNode(ll,scheduledProcess);
                scheduledProcess = NULL;
            }
            
        } else { //if no schedular is running
            printf("_");
        }
        
        // increase the time
        currentTime++;
    }

    // print the Stat of the process
    return printPolicyStat(ll);
}

