#include <stdio.h>
#include <stdlib.h>
#include "stat.h"
#include "common.h"

process_stat *generateProcess(process *proc);
int comparePriority(void *data1, void *data2);
void printqueue(queue *q);

// Function to dequeue a specific process from the queue
int dequeueSpecificNP(queue *q, process_stat *procStat) {
    if (q->head == NULL) return 0; // Queue is empty
    
    node *current = q->head;
    node *previous = NULL;

    while (current != NULL) {
        process_stat *currentProc = (process_stat *)current->data;
        
        // If this is the process we want to remove
        if (currentProc == procStat) {
            if (previous == NULL) {
                // Removing the head of the queue
                q->head = current->next;
            } else {
                // Removing a middle or last element
                previous->next = current->next;
            }
            if (current == q->tail) {
                // Removing the tail
                q->tail = previous;
            }
            free(current);
            q->size--;
            return 1; // Successfully removed
        }
        previous = current;
        current = current->next;
    }
    return 0; // Process not found
}

// Helper function to increment the waiting times of processes in the queue
void incrementWaitingTimesNP(queue *q) {
    node *current = q->head;
    while (current != NULL) {
        process_stat *procStat = (process_stat *)current->data;
        procStat->waitingTime++;  // Increment waiting time for each process
        current = current->next;
    }
}

// Helper function to apply aging: promote processes to higher priority queues
void applyAgingNP(queue *fromQueue, queue *toQueue) {
    node *current = fromQueue->head;
    while (current != NULL) {
        process_stat *procStat = (process_stat *)current->data;
        if (procStat->waitingTime >= 5) {  // If the process waited for 5 quanta
            dequeueSpecificNP(fromQueue, procStat);  // Remove from current queue
            enqueue(toQueue, procStat);            // Move to higher-priority queue
            procStat->waitingTime = 0;             // Reset waiting time after promotion
        }
        current = current->next;
    }
}

average_stats highestPriorityFirstNonPreemptiveAging(linked_list *processes) {
    int currentTime = 0;  // quanta
    // Create a queue of processes.
    queue *processQueue1 = createQueue();
    queue *processQueue2 = createQueue();
    queue *processQueue3 = createQueue();
    queue *processQueue4 = createQueue();

    // Create linked lists to store results of statistics
    linked_list *linkedlist1 = createLinkedList();
    linked_list *linkedlist2 = createLinkedList();
    linked_list *linkedlist3 = createLinkedList();
    linked_list *linkedlist4 = createLinkedList();

    node *processPointer = processes->head;
    if (processes->head == NULL) {
        fprintf(stderr, "There is no Process to schedule\n");
    }
    printf("\n\n\n==================================================================================================================================\n");
    printf("\n\n\n==================================================================================================================================\n");
    printf("\nHIGHEST PRIORITY FIRST NON PREEMPTIVE AGING::\n");
    printf("Order of Processes: ");

    // Scheduling loop
    process_stat *scheduledProcess = NULL;
    while (currentTime < 100 || scheduledProcess != NULL) {
        // Check for incoming new processes and enqueue based on priority
        if (processPointer != NULL) {
            process *newProcess = (process *)(processPointer->data);

            // Enqueue new processes that have arrived by current time
            while (processPointer != NULL && newProcess->arrivalTime <= currentTime) {
                if (newProcess->priority == 1) 
                    enqueue(processQueue1, generateProcess(newProcess));
                else if (newProcess->priority == 2) 
                    enqueue(processQueue2, generateProcess(newProcess));
                else if (newProcess->priority == 3) 
                    enqueue(processQueue3, generateProcess(newProcess));
                else if (newProcess->priority == 4) 
                    enqueue(processQueue4, generateProcess(newProcess));
                
                processPointer = processPointer->next;
                if (processPointer != NULL) {
                    newProcess = (process *)(processPointer->data);
                }
            }
        }

        // Increment waiting times for processes in all queues
        incrementWaitingTimesNP(processQueue1);
        incrementWaitingTimesNP(processQueue2);
        incrementWaitingTimesNP(processQueue3);
        incrementWaitingTimesNP(processQueue4);

        // Apply aging to promote processes to higher-priority queues
        applyAgingNP(processQueue2, processQueue1);
        applyAgingNP(processQueue3, processQueue2);
        applyAgingNP(processQueue4, processQueue3);

        // If no process is currently scheduled, get the next highest priority process
        if (scheduledProcess == NULL) {
            if (processQueue1->size > 0)
                scheduledProcess = (process_stat *)dequeue(processQueue1);
            else if (processQueue2->size > 0)
                scheduledProcess = (process_stat *)dequeue(processQueue2);
            else if (processQueue3->size > 0)
                scheduledProcess = (process_stat *)dequeue(processQueue3);
            else if (processQueue4->size > 0)
                scheduledProcess = (process_stat *)dequeue(processQueue4);

            // Handle if process arrives after time 100
            if (currentTime >= 100 && scheduledProcess->startTime == -1) {
                scheduledProcess = NULL;
                continue;
            }
        }

        // Execute the scheduled process
        if (scheduledProcess != NULL) {
            process *proc = scheduledProcess->proc;
            printf("%c", proc->pid);  // Add the currently running process to the time chart

            // Update the current process stats
            if (scheduledProcess->startTime == -1) {
                scheduledProcess->startTime = currentTime;
            }

            scheduledProcess->runTime++;

            if (scheduledProcess->runTime >= proc->runTime) {
                scheduledProcess->endTime = currentTime;

                // Add nodes to the linked list based on priority
                if (scheduledProcess->proc->priority == 1) 
                    addNode(linkedlist1, scheduledProcess);
                else if (scheduledProcess->proc->priority == 2) 
                    addNode(linkedlist2, scheduledProcess);
                else if (scheduledProcess->proc->priority == 3) 
                    addNode(linkedlist3, scheduledProcess);
                else if (scheduledProcess->proc->priority == 4) 
                    addNode(linkedlist4, scheduledProcess);

                scheduledProcess = NULL;
            }
        } else {
            printf("_");  // No process is scheduled, print idle state
        }

        // Increment the current time
        currentTime++;
    }

    // Print process statistics
    average_stats avg1, avg2, avg3, avg4, avg;
    printf("\nFor the  Priority Queue 1");
    avg1 = printPolicyStat(linkedlist1);
    printf("\nFor the  Priority Queue 2");
    avg2 = printPolicyStat(linkedlist2);
    printf("\nFor the  Priority Queue 3");
    avg3 = printPolicyStat(linkedlist3);
    printf("\nFor  the Priority Queue 4");
    avg4 = printPolicyStat(linkedlist4);

    // Calculate the mean response time, wait time, turnaround time, and throughput
    avg.avg_response_time = (avg1.avg_response_time + avg2.avg_response_time + avg3.avg_response_time + avg4.avg_response_time) / 4;
    avg.avg_wait_time = (avg1.avg_wait_time + avg2.avg_wait_time + avg3.avg_wait_time + avg4.avg_wait_time) / 4;
    avg.avg_turnaround = (avg1.avg_turnaround + avg2.avg_turnaround + avg3.avg_turnaround + avg4.avg_turnaround) / 4;
    avg.avg_throughput = (avg1.avg_throughput + avg2.avg_throughput + avg3.avg_throughput + avg4.avg_throughput);

    printf("\nTHE AVERAGE TIME FOR HIGH PRIORITY FIRST NON-PREEMPTIVE AGING ALL QUEUES::\n");
    printf("|------------------------------------------------------------|\n");
    printf("| Statistics                  | Average                 |\n");
    printf("|---------------------------|--------------------------------|\n");
    printf("| Response-Time     | %.1f                           |\n", avg.avg_response_time);
    printf("| Wait-Time         | %.1f                           |\n", avg.avg_wait_time);
    printf("| Turn-Around-Time  | %.1f                           |\n", avg.avg_turnaround);
    printf("|------------------------------------------------------------|\n");

    return avg;
}
