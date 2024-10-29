#include "stat.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>

process_stat *generateProcess(process *proc);

// Function to compare the priority and arrival time
int priorityComparison(void *data1, void *data2)
{
  process_stat *proc1 = (process_stat *) data1;
  process_stat *proc2 = (process_stat *) data2;
  
  // Compare the priorities of both the processes
  if(((((process *)proc1->proc)->priority)) < ((((process *)proc2->proc)->priority))) {
    return -1;
  } else if (((((process *)proc1->proc)->priority)) == ((((process *)proc2->proc)->priority)) 
             &&  (((process *)proc1->proc)->arrivalTime < (((process *)proc2->proc)->arrivalTime))) {
    return -1;
  } else {
    return 1;
  }
}

// Implementation of Preemptive HPF with Round Robin (Time Slice = 1 Quantum)
average_stats highestPriorityFirstPreemptive(linked_list *procs)
{
  int currentTime = 0;
  int timeQuantum = 1;  // Set time slice = 1 quantum
  
  // Creation of 4 Linked Lists for managing order of processes in preemption
  linked_list *linkedList1 = createLinkedList();
  linked_list *linkedList2 = createLinkedList();
  linked_list *linkedList3 = createLinkedList();
  linked_list *linkedList4 = createLinkedList();

  // Creation of 4 process queues used to store processes based on priority
  queue *proc_queue1 = createQueue();
  queue *proc_queue2 = createQueue();
  queue *proc_queue3 = createQueue();
  queue *proc_queue4 = createQueue();

  node *proc_pointer = procs->head;
  if (procs->head == NULL) {
    fprintf(stderr, "There is no Process to schedule\n");
  }
  printf("\n\n\n==================================================================================================================================\n");
  printf("\n\n\n==================================================================================================================================\n");

  printf("\nHIGHEST PRIORITY FIRST PREEMPTIVE WITH ROUND ROBIN (Time Slice = 1)::\n");
  printf("Order of Processes: ");

  // Check time currentTime is less than 100 or there is a scheduled process
  process_stat *scheduledProcess = NULL;
  int quantumCounter = 0;

  while (currentTime < 100 || scheduledProcess != NULL) 
  {
    if (scheduledProcess != NULL) {
      quantumCounter++;
      
      // Enqueue based on priority after every quantum
      if (quantumCounter == timeQuantum) {
        if (scheduledProcess->proc->priority == 1)
          enqueue(proc_queue1, scheduledProcess);
        else if (scheduledProcess->proc->priority == 2)
          enqueue(proc_queue2, scheduledProcess);
        else if (scheduledProcess->proc->priority == 3)
          enqueue(proc_queue3, scheduledProcess);
        else if (scheduledProcess->proc->priority == 4)
          enqueue(proc_queue4, scheduledProcess);
        
        scheduledProcess = NULL;
        quantumCounter = 0;
      }
    }

    // If new process comes, we enqueue it in the queue
    if (proc_pointer != NULL) {
      process *newProcess = (process *)(proc_pointer->data);
      
      // Check new process arrival time
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

    // Sort the queues based on priority comparison
    sort(proc_queue1, priorityComparison);
    sort(proc_queue2, priorityComparison);
    sort(proc_queue3, priorityComparison);
    sort(proc_queue4, priorityComparison);

    // If there are no scheduled processes, we check process queue and schedule it
    if (scheduledProcess == NULL) {
      if (proc_queue1->size > 0) 
        scheduledProcess = (process_stat *) dequeue(proc_queue1);
      else if (proc_queue2->size > 0) 
        scheduledProcess = (process_stat *) dequeue(proc_queue2);
      else if (proc_queue3->size > 0) 
        scheduledProcess = (process_stat *) dequeue(proc_queue3);
      else if (proc_queue4->size > 0) 
        scheduledProcess = (process_stat *) dequeue(proc_queue4);

      // If the process has not started before currentTime 100, we remove it from the queue
      if (currentTime >= 100 && scheduledProcess != NULL && scheduledProcess->startTime == -1) {
        scheduledProcess = NULL;
        continue;
      }
    }

    if (scheduledProcess != NULL) {
      process *proc = scheduledProcess->proc;

      // Add the currently running process to the time chart
      printf("%c", proc->pid);

      // Update the current process stat
      if (scheduledProcess->startTime == -1) {
        scheduledProcess->startTime = currentTime;
      }

      scheduledProcess->runTime++;
      
      // If the process has finished, add it to the final list
      if (scheduledProcess->runTime >= proc->runTime) {
        scheduledProcess->endTime = currentTime;

        // Adding to the linked list based on priority
        if (scheduledProcess->proc->priority == 1)
          addNode(linkedList1, scheduledProcess);
        else if (scheduledProcess->proc->priority == 2)
          addNode(linkedList2, scheduledProcess);
        else if (scheduledProcess->proc->priority == 3)
          addNode(linkedList3, scheduledProcess);
        else if (scheduledProcess->proc->priority == 4)
          addNode(linkedList4, scheduledProcess);

        scheduledProcess = NULL;
        quantumCounter = 0;
      }
    } else {
      printf("_");
    }

    // Increase the currentTime
    currentTime++;
  }

  // Print Process Statistics
  average_stats avg1, avg2, avg3, avg4, avg;
  printf("\nFor the Priority Queue 1");
  avg1 = printPolicyStat(linkedList1);
  printf("\nFor the Priority Queue 2");
  avg2 = printPolicyStat(linkedList2);
  printf("\nFor the Priority Queue 3");
  avg3 = printPolicyStat(linkedList3);
  printf("\nFor the Priority Queue 4");
  avg4 = printPolicyStat(linkedList4);

  avg.avg_response_time = (avg1.avg_response_time + avg2.avg_response_time + avg3.avg_response_time + avg4.avg_response_time) / 4;
  avg.avg_wait_time = (avg1.avg_wait_time + avg2.avg_wait_time + avg3.avg_wait_time + avg4.avg_wait_time) / 4;
  avg.avg_turnaround = (avg1.avg_turnaround + avg2.avg_turnaround + avg3.avg_turnaround + avg4.avg_turnaround) / 4;
  avg.avg_throughput = (avg1.avg_throughput + avg2.avg_throughput + avg3.avg_throughput + avg4.avg_throughput);

  printf("\nTHE AVERAGE TIME FOR HIGH PRIORITY FIRST PREEMPTIVE WITH ROUND ROBIN (Time Slice = 1) ALL QUEUES::\n");
  printf("|------------------------------------------------------------|\n");
  printf("| Statistics              | Average                          |\n");
  printf("|-------------------------|----------------------------------|\n");
  printf("| Response-Time           | %.1f                             |\n", avg.avg_response_time);
  printf("| Wait-Time               | %.1f                             |\n", avg.avg_wait_time);
  printf("| Turn-Around-Time        | %.1f                             |\n", avg.avg_turnaround);
  printf("|------------------------------------------------------------|\n");

  return avg;
}
