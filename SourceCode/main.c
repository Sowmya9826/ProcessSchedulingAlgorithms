#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "process.h"
#include "stat.h"


// Define a method to update result values based on a given scheduling algorithm
void updateResult(int resultIndex, average_stats *resultArr, average_stats *algorithm) {
    for (int i = 0; i < 5; i++) {
        resultArr[resultIndex].avg_response_time += algorithm[i].avg_response_time;
        resultArr[resultIndex].avg_wait_time += algorithm[i].avg_wait_time;
        resultArr[resultIndex].avg_turnaround += algorithm[i].avg_turnaround;
        resultArr[resultIndex].avg_throughput += algorithm[i].avg_throughput;
    }
}

void printProcessTable(linked_list *processList) {
    printf("-------------------------------------------------\n");
    printf("Process-ID | Arrival-Time | Run-Time | Priority |\n");
    printf("-------------------------------------------------\n");
    node *ptr = processList->head;
    while(ptr != NULL) {
        process *proc = (process *)ptr->data;
        printf("%10c | %12d | %8d | %8d |\n", proc->pid, proc->arrivalTime, proc->runTime, proc->priority);
        ptr = ptr->next;
    }
    printf("--------------------------------------------------\n");
    printf("Total Processes : %d\n", processList->size);
    printf("--------------------------------------------------\n");
}

void printAlgorithmStats(const char *algorithmName, average_stats *stats) {
    printf("\n");
    printf("------------------------------------------------------------------------------------------------\n");
    printf("|                         %-69s |\n", algorithmName);
    printf("|---------------------------|---------------------------|-------------------|-------------------|\n");
    printf("| %-25s | %-25s | %-17s| %-17s|\n", "Average-Response-Time", "Average-Wait-Time", "Average-Turnaround", "Average-Throughput");
    printf("|---------------------------|---------------------------|-------------------|-------------------|\n");
    printf("| %-25.1f | %-25.1f | %-17.1f | %-17.1f |\n", stats->avg_response_time, stats->avg_wait_time, stats->avg_turnaround, stats->avg_throughput);
    printf("|---------------------------|---------------------------|-------------------|-------------------|\n");
    printf("\n");
}


int main(int argc, char **argv) {
    int counter = 0;
    int time_slice = 1; // For RR, use a time slice of 1 quantum.
    printf("Round Robin Time Slice: %d\n", time_slice);
    average_stats fcfs[5], sjf[5], srf[5], rr[5], hpfp[5], hpfnp[5], hpfp_aging[5], hpfnp_aging[5], result[8];

    while (counter < 5) {
        // Generate Processes
        printf("\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
        printf("\t\t\t\tROUND %d\n", counter + 1);
        printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

        linked_list *processList = generateProcesses(52);
        printProcessTable(processList);

        // First Come First Serve
        fcfs[counter] = firstComeFirstServeNonPreemptive(processList);
        // Shortest Job First
        sjf[counter] = shortestJobFirstNonPreemptive(processList);
        // Shortest Remaining Time First
        srf[counter] = shortestRemainingTimePreemptive(processList);
        // Round Robin
        if (argc > 1) {
            time_slice = atoi(argv[1]);
        }
        printf("Round Robin Time Slice: %d\n", time_slice);
        rr[counter] = roundRobinPreemptive(processList, time_slice);
        // Highest Priority First Preemptive
        hpfp[counter] = highestPriorityFirstPreemptive(processList);
        // Highest Priority First Preemptive - Aging
        hpfp_aging[counter] = highestPriorityFirstPreemptiveAging(processList);
        // Highest Priority First Non-Preemptive
        hpfnp[counter] = highestPriorityFirstNonPreemptive(processList);
        // Highest Priority First Non-Preemptive - Aging
        hpfnp_aging[counter] = highestPriorityFirstNonPreemptiveAging(processList);
        counter++;
    }

    for (int i = 0; i < 6; i++) {
        result[i].avg_response_time = 0;
        result[i].avg_wait_time = 0;
        result[i].avg_turnaround = 0;
        result[i].avg_throughput = 0;
    }

    // Total of all average values    
    updateResult(0, result, fcfs);
    updateResult(1, result, sjf);
    updateResult(2, result, srf);
    updateResult(3, result, rr);
    updateResult(4, result, hpfp);
    updateResult(5, result, hpfnp);
    updateResult(6, result, hpfp_aging);
    updateResult(7, result, hpfnp_aging);


    // Average for all the algorithms
    for (int i = 0; i < 8; i++) {
        result[i].avg_response_time /= 5;
        result[i].avg_wait_time /= 5;
        result[i].avg_turnaround /= 5;
        result[i].avg_throughput /= 5;
    }

    printf("\n\n\n");
    printf("///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n\n");
    printf("///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n\n");
    
    printf("The average metrics obtained from 5 runs for each algorithm:\n");
    printf("\n");

    printAlgorithmStats("First Come First Served (non-preemptive)", &result[0]);
    printAlgorithmStats("Shortest Job First (non-preemptive)", &result[1]);
    printAlgorithmStats("Shortest Remaining Time (preemptive)", &result[2]);
    printAlgorithmStats("Round Robin (preemptive)", &result[3]);
    printAlgorithmStats("Highest Priority First (preemptive)", &result[4]);
    printAlgorithmStats("Highest Priority First (preemptive) - Aging", &result[6]);
    printAlgorithmStats("Highest Priority First (non-preemptive)", &result[5]);
    printAlgorithmStats("Highest Priority First (non-preemptive) - Aging", &result[7]);
 
    return 0;
}
