#include <stdio.h>
#include <stdlib.h>
#include "stat.h"
#include "common.h"

process_stat *generateProcess(process* proc);

int compareRunTime(void *data1, void *data2) {
    process_stat *prs1 = (process_stat *) data1;
    process_stat *prs2 = (process_stat *) data2;
    if(((process *)prs1->proc)->runTime < ((process *)prs2->proc)->runTime) {
        return -1;
    } else {
        return 1;
    }
}

average_stats shortestJobFirstNonPreemptive(linked_list *processes) {
    int currentTime = 0;

    //Create Process Queue
    queue *processQueue = (queue *)createQueue();
    node *procPtr = processes->head;
    if(processes->head == NULL) {
        fprintf(stderr,"No Process scheduleD\n");
    }

    process_stat *scheduledProcess = NULL;

    linked_list *ll = createLinkedList();
    printf("\n\n\n==================================================================================================================================\n");
    printf("\n\n\n==================================================================================================================================\n");

    printf("\nSHORTEST JOB FIRST NON PREEMPTIVE::\n");
    printf("Order of Processes: ");
    
    //check process queue is empty or the currentTime quanta is less than 100.
    while(scheduledProcess!=NULL || currentTime<100 ) {
        //check incoming new process and enqueue it in the queue
        if(procPtr != NULL) {
            process *newProcess = (process *)(procPtr->data);
            while(procPtr!=NULL && newProcess->arrivalTime <= currentTime) {
                enqueue(processQueue,generateProcess(newProcess));
                sort(processQueue,compareRunTime);
                procPtr = procPtr->next;
                if(procPtr!=NULL)
                    newProcess = (process *)(procPtr->data);
            }
        }

        // If there is no scheduled process then check the process queue and schedule it.
        if(scheduledProcess == NULL && processQueue->size > 0) {
            scheduledProcess = (process_stat *) dequeue(processQueue);
        }

        if(scheduledProcess != NULL) {
            process * proc = scheduledProcess->proc;

            //add the current running process to the currentTime 
            printf("%c",proc->pid);

            //update the current processes stat
            if(scheduledProcess->startTime == -1) {
                scheduledProcess->startTime = currentTime;
            }
            scheduledProcess->runTime++;

            if(scheduledProcess->runTime >= proc->runTime) {
                scheduledProcess->endTime = currentTime;
                //adding the scheduled process
                addNode(ll,scheduledProcess);
                scheduledProcess = NULL;
            }
        } else {
            printf("_");
        }
        //IncreasE the time
        currentTime++;
    }
    printf("\n");

    //Print the stat of the process
    return printPolicyStat(ll);
    
}

