#include <stdio.h>
#include <stdlib.h>
#include "stat.h"
#include "common.h"

process_stat *generateProcess(process *proc);

average_stats roundRobinPreemptive(linked_list *processes, int time_slice) {
    int currentTime = 0;

    // Creation of Process Queue
    queue *process_queue = (queue *)createQueue();
    node *process_pointer = processes->head;
    if (processes->head == NULL) {
        fprintf(stderr, "No Process to schedule\n");
    }
    
    process_stat *scheduledProcess = NULL;

    linked_list *time_chart = createLinkedList();
	printf("\n\n\n==================================================================================================================================\n");
    printf("\nROUND ROBIN PREEMPTIVE::\n");
    printf("Order of Processes: ");
    node *current_node = NULL;
    int current_run_time = 0;

    while (currentTime < 100 || process_queue->size > 0) {

        // Check for new process and enqueue.
        if (process_pointer != NULL && currentTime < 100) {
            process *new_process = (process *)(process_pointer->data);
            while (process_pointer != NULL && new_process->arrivalTime <= currentTime) {
                enqueue(process_queue, generateProcess(new_process));
                process_pointer = process_pointer->next;
                if (process_pointer != NULL)
                    new_process = (process *)(process_pointer->data);
            }
        }

        // Check process queue if there is no scheduled process 
        if (current_node == NULL) {
            current_run_time = 0;
            current_node = process_queue->head;
        } else if (current_run_time == time_slice) {
            current_run_time = 0;
            current_node = current_node->next;
            if (current_node == NULL) {
                current_node = process_queue->head;
            }
        }

        if (current_node != NULL) {
            scheduledProcess = (process_stat *)current_node->data;
            process *proc = scheduledProcess->proc;

            if (currentTime >= 100) {
                if (scheduledProcess->startTime == -1) {
                    // Do not start any new process, remove it from process_queue
                    free(scheduledProcess);
                    node *next = current_node->next;
                    removeNode(process_queue, current_node->data);
                    current_node = next;
                    current_run_time = 0;
                    continue;
                }
            }
            
            // Add the currently running process to the time chart
            printf("%c", proc->pid);
            current_run_time++;

            // Update the current process stat
            if (scheduledProcess->startTime == -1) {
                scheduledProcess->startTime = currentTime;
            }
            scheduledProcess->runTime++;

            if (scheduledProcess->runTime >= proc->runTime) {
                scheduledProcess->endTime = currentTime;
                addNode(time_chart, scheduledProcess);
                node *next = current_node->next;
                removeNode(process_queue, current_node->data);
                current_node = next;
                current_run_time = 0;
            }
        } else {
            printf("_");
        }

        // increas the time
        currentTime++;
    }

    return printPolicyStat(time_chart);
}
