#ifndef _stat_h_
#define _stat_h_
#include "process.h"

enum State {UNUSED, ZOMBIE,EMBRYO, SLEEPING, RUNNING};

struct process_stat_s {
	process * proc;
	float waitingTime;
	float turnaroundTime;
	float responseTime;
	float startTime;
	float endTime;
	float runTime;
	enum State state;
};

typedef struct process_stat_s process_stat;

struct stat_s {
	linked_list * processQueue;
	linked_list * time_chart;
	float avg_turnaround_time;
	float avg_waiting_time;
	float avg_response_time;
	float throughput;
	int total_quanta;
};

typedef struct stat_s scheduling_stat;

struct average_stats_s{
	float avg_response_time;
	float avg_wait_time;
	float avg_turnaround;
	float avg_throughput;
};

typedef struct average_stats_s average_stats;

average_stats firstComeFirstServeNonPreemptive(linked_list * processes);
average_stats shortestJobFirstNonPreemptive(linked_list * processes);
average_stats shortestRemainingTimePreemptive(linked_list * processes);
average_stats roundRobinPreemptive(linked_list * processes,int time_slice);
average_stats highestPriorityFirstPreemptive(linked_list * processes);
average_stats highestPriorityFirstPreemptiveAging(linked_list * processes);
average_stats highestPriorityFirstNonPreemptive(linked_list * processes);
average_stats highestPriorityFirstNonPreemptiveAging(linked_list * processes);


// To print policy stat
average_stats printPolicyStat(linked_list * ll);

#endif
