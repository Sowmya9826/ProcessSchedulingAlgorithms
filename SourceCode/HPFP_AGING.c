#include "stat.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>

// Function to dequeue a specific process from the queue
int dequeueSpecific(queue *q, process_stat *procStat) {
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


process_stat *generateProcess(process *proc);

// Function to compare the priority and arrival time
int priorityComparisonAging(void *data1, void *data2) {
    process_stat *proc1 = (process_stat *)data1;
    process_stat *proc2 = (process_stat *)data2;
    
    // Compare the priorities of both the processes
    if (((process *)proc1->proc)->priority < ((process *)proc2->proc)->priority) {
        return -1;
    }
    else if (((process *)proc1->proc)->priority == ((process *)proc2->proc)->priority && ((process *)proc1->proc)->arrivalTime < ((process *)proc2->proc)->arrivalTime) {
        return -1;
    }
    else {
        return 1;
    }
}

// Helper function to increment waiting times for all processes in a given queue
void incrementWaitingTimes(queue *q) {
    node *current = q->head;
    while (current != NULL) {
        process_stat *procStat = (process_stat *)current->data;
        procStat->waitingTime++; // Increment waiting time for each process
        current = current->next;
    }
}

// Helper function to apply aging: move processes to a higher priority queue if they have waited too long
void applyAging(queue *fromQueue, queue *toQueue) {
    node *current = fromQueue->head;
    while (current != NULL) {
        process_stat *procStat = (process_stat *)current->data;
        if (procStat->waitingTime >= 5) { // If waiting time exceeds 5 quanta
            dequeueSpecific(fromQueue, procStat); // Remove from current queue
            enqueue(toQueue, procStat);           // Move to higher-priority queue
            procStat->waitingTime = 0;            // Reset waiting time after promotion
        }
        current = current->next;
    }
}

// Implementation of the HPF preemptive scheduling with aging
average_stats highestPriorityFirstPreemptiveAging(linked_list *procs) {
    int currentTime = 0;
  
    // Create 4 linked lists to manage process order in preemption
    linked_list *linkedList1 = createLinkedList();
    linked_list *linkedList2 = createLinkedList();
    linked_list *linkedList3 = createLinkedList();
    linked_list *linkedList4 = createLinkedList();

    // Create 4 process queues to store processes based on priority
    queue *proc_queue1 = createQueue();
    queue *proc_queue2 = createQueue();
    queue *proc_queue3 = createQueue();
    queue *proc_queue4 = createQueue();

    node *proc_pointer = procs->head;
    if (procs->head == NULL) {
        fprintf(stderr, "There is no process to schedule\n");
        return (average_stats){0};
    }

    printf("\n\n\n==================================================================================================================================\n");
    printf("\n\n\n==================================================================================================================================\n");
    printf("\nHIGHEST PRIORITY FIRST PREEMPTIVE AGING::\n");
    printf("Order of Processes: ");

    // Main scheduling loop
    process_stat *scheduledProcess = NULL;
    while (currentTime < 100 || scheduledProcess != NULL) {
        // Enqueue the currently running process back to its appropriate queue
        if (scheduledProcess != NULL) {
            if (scheduledProcess->proc->priority == 1) 
                enqueue(proc_queue1, scheduledProcess);
            else if (scheduledProcess->proc->priority == 2) 
                enqueue(proc_queue2, scheduledProcess);
            else if (scheduledProcess->proc->priority == 3) 
                enqueue(proc_queue3, scheduledProcess);
            else if (scheduledProcess->proc->priority == 4) 
                enqueue(proc_queue4, scheduledProcess);
        }

        // If a new process has arrived, enqueue it based on its priority
        if (proc_pointer != NULL) {
            process *newProcess = (process *)(proc_pointer->data);
            while (proc_pointer != NULL && newProcess->arrivalTime <= currentTime) {
                if (newProcess->priority == 1) 
                    enqueue(proc_queue1, generateProcess(newProcess));
                if (newProcess->priority == 2) 
                    enqueue(proc_queue2, generateProcess(newProcess));
                if (newProcess->priority == 3) 
                    enqueue(proc_queue3, generateProcess(newProcess));
                if (newProcess->priority == 4) 
                    enqueue(proc_queue4, generateProcess(newProcess));
                
                proc_pointer = proc_pointer->next;
                if (proc_pointer != NULL) {
                    newProcess = (process *)(proc_pointer->data);
                }
            }
        }

        // Increment waiting times for processes in all queues
        incrementWaitingTimes(proc_queue1);
        incrementWaitingTimes(proc_queue2);
        incrementWaitingTimes(proc_queue3);
        incrementWaitingTimes(proc_queue4);

        // Apply aging to move processes to higher-priority queues if needed
        applyAging(proc_queue2, proc_queue1);  // Move from queue 2 to 1
        applyAging(proc_queue3, proc_queue2);  // Move from queue 3 to 2
        applyAging(proc_queue4, proc_queue3);  // Move from queue 4 to 3

        // Sort queues after applying aging
        sort(proc_queue1, priorityComparisonAging);
        sort(proc_queue2, priorityComparisonAging);
        sort(proc_queue3, priorityComparisonAging);
        sort(proc_queue4, priorityComparisonAging);

        // Schedule the next process from the highest priority queue
        if (scheduledProcess == NULL) {
            if (proc_queue1->size > 0) 
                scheduledProcess = (process_stat *)dequeue(proc_queue1);
            else if (proc_queue2->size > 0) 
                scheduledProcess = (process_stat *)dequeue(proc_queue2);
            else if (proc_queue3->size > 0) 
                scheduledProcess = (process_stat *)dequeue(proc_queue3);
            else if (proc_queue4->size > 0) 
                scheduledProcess = (process_stat *)dequeue(proc_queue4);
        }

        // Process the scheduled process
        if (scheduledProcess != NULL) {
            process *proc = scheduledProcess->proc;
            printf("%c", proc->pid);  // Print process ID for time chart

            // Update the process statistics
            if (scheduledProcess->startTime == -1) {
                scheduledProcess->startTime = currentTime;
            }

            scheduledProcess->runTime++;

            // If the process has finished its execution
            if (scheduledProcess->runTime >= proc->runTime) {
                scheduledProcess->endTime = currentTime;

                // Add the process to the corresponding linked list based on priority
                if (scheduledProcess->proc->priority == 1) 
                    addNode(linkedList1, scheduledProcess);
                else if (scheduledProcess->proc->priority == 2) 
                    addNode(linkedList2, scheduledProcess);
                else if (scheduledProcess->proc->priority == 3) 
                    addNode(linkedList3, scheduledProcess);
                else if (scheduledProcess->proc->priority == 4) 
                    addNode(linkedList4, scheduledProcess);
                
                scheduledProcess = NULL;  // Clear the scheduled process
            }
        } else {
            printf("_");  // No process is scheduled, print idle state
        }

        // Increment the current time
        currentTime++;
    }

    // Print process statistics
    average_stats avg1, avg2, avg3, avg4, avg;
    printf("\nFor the Priority Queue 1");
    avg1 = printPolicyStat(linkedList1);
    printf("\nFor the Priority Queue 2");
    avg2 = printPolicyStat(linkedList2);
    printf("\nFor the Priority Queue 3");
    avg3 = printPolicyStat(linkedList3);
    printf("\nFor the Priority Queue 4");
    avg4 = printPolicyStat(linkedList4);

    // Calculate average statistics
    avg.avg_response_time = (avg1.avg_response_time + avg2.avg_response_time + avg3.avg_response_time + avg4.avg_response_time) / 4;
    avg.avg_wait_time = (avg1.avg_wait_time + avg2.avg_wait_time + avg3.avg_wait_time + avg4.avg_wait_time) / 4;
    avg.avg_turnaround = (avg1.avg_turnaround + avg2.avg_turnaround + avg3.avg_turnaround + avg4.avg_turnaround) / 4;
    avg.avg_throughput = (avg1.avg_throughput + avg2.avg_throughput + avg3.avg_throughput + avg4.avg_throughput);

    printf("\nTHE AVERAGE TIME FOR HIGH PRIORITY FIRST PREEMPTIVE AGING ALL QUEUES::\n");
    printf("|------------------------------------------------------------|\n");
    printf("| Statistics                  | Average                   |\n");
    printf("|---------------------------|--------------------------------|\n");
    printf("| Response-Time     | %.1f                           |\n", avg.avg_response_time);
    printf("| Wait-Time         | %.1f                           |\n", avg.avg_wait_time);
    printf("| Turn-Around-Time  | %.1f                           |\n", avg.avg_turnaround);
    printf("|------------------------------------------------------------|\n");

    return avg;
}
