#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "process.h"
#include "stat.h"

// create a new linked_list
linked_list *createLinkedList() {
	linked_list *new_ll = (linked_list *) malloc(sizeof(linked_list));
	new_ll->head = NULL;
	new_ll->tail = NULL;
	new_ll->size = 0;
	return new_ll;
}

// create a new node
node *createNode(void *data) {
	node *new_node = (node *) malloc(sizeof(node));
	new_node->data = data;
	new_node->next = NULL;
	new_node->prev = NULL;
	return new_node;
}

// add a node to an existing linked list
void addNode(linked_list *ll, void *data) {
    if (ll == NULL) return;

    node *new_node = createNode(data);
    if (new_node == NULL) return;

    if (ll->size == 0) {
        ll->head = new_node;
        ll->tail = new_node;
    } else {
        new_node->prev = ll->tail;
        ll->tail->next = new_node;
        ll->tail = new_node;
    }
    ll->size++;
}

// remove a node from an existing linked list
void removeNode(linked_list *ll, void *data) {
    if (ll == NULL || ll->size == 0) return;

    node *currentNode = ll->head;

    while (currentNode != NULL && currentNode->data != data) {
        currentNode = currentNode->next;
    }

    if (currentNode != NULL) {
        if (currentNode->prev != NULL) {
            currentNode->prev->next = currentNode->next;
        } else {
            ll->head = currentNode->next;
        }

        if (currentNode->next != NULL) {
            currentNode->next->prev = currentNode->prev;
        } else {
            ll->tail = currentNode->prev;
        }

        free(currentNode);
        ll->size--;
    }
}

// remove a node from an existing linked list
void removeHead(linked_list *ll) {
	node *currentNode = ll->head;
	
    if(currentNode != NULL) {
		ll->head = currentNode->next;
		if(ll->tail == currentNode) {
			ll->tail = currentNode->prev;
		}
		ll->size --;
		free(currentNode);
	}
}

// add a new node after a particular node in an existing linked list
void addAfter(linked_list *ll, node *after_node, void *data) {
	node *new_node = createNode(data);
	node *next_node = after_node->next;
	new_node->next = next_node;
    
	if (next_node != NULL) next_node->prev = new_node;

	new_node->prev = after_node;
	after_node->next = new_node;

	if (ll->tail == after_node) {
		ll->tail = new_node;
	}

	ll->size++;
}

void sort(linked_list *ll, int (*cmp)(void *data1, void *data2)) {
	node *i = ll->head;
    
	while (i!=NULL) {
		node *j = i->next;
		while (j!=NULL) {
			void *p1 = i->data;
			void *p2 = j->data;
			
            if ((*cmp)(p1,p2) > 0) {
				swapNodes(i,j);
			}
            
			j=j->next;
		}
		i = i->next;
	}
}

void swapNodes(node *a, node *b) {
	void *temp = a->data;
	a->data = b->data;
	b->data = temp;
}

// Queue Implementation
queue *createQueue() {
	return createLinkedList();
}

// The enqueue function to add process at the end of the queue
void enqueue(queue *q, void *data) {
	node *new_node = createNode(data);
	new_node->prev = q->tail;
    
	if(q->tail != NULL) {
		q->tail->next = new_node;
		q->tail = new_node;
	} else {
		q->tail = new_node;
		q->head = new_node;
	}
	q->size += 1;
}

// The dequeue function to remove process from the end of the queue
void *dequeue(queue *q) {
	if (q->head != NULL) {
        //Address of q->head
		node *currentNode = q->head;
		void *data = currentNode->data;

		// To move head to next node
		node *next_node = q->head->next;

		if(next_node != NULL) next_node->prev = NULL;
        
        //currentNode = q->head = next_node
		q->head = next_node;

		// To maintain boundary tail condition
		if(q->tail == currentNode) {
			q->tail = NULL;
		}

		q->size--;
		free(currentNode);
		return data;
	}
	return NULL;
}

average_stats printPolicyStat(linked_list *ll) {
	average_stats avg;
    
	printf("\n");
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");
	printf("Process-Name\t| Arrival-Time | Start-Time | End-Time | Run-Time | Response-Time | Wait-Time | Turn-Around-Time | Priority |\n");
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");
	
    node *ptr = ll->head;
	float avg_response_time = 0;
	float avg_wait_time = 0;
	float avg_turnaround = 0;
	int process_count = 0;
    
	while (ptr!=NULL) {
		process_stat *stat = (process_stat *)ptr->data;
		if (stat == NULL) printf("No Stat\n");
		process *proc = (process *)stat->proc;
		if (proc == NULL) printf("No Process\n");
		float arrivalTime = proc->arrivalTime;
		float runTime = proc->runTime;
		float responseTime = stat->startTime - arrivalTime;
		float turnaround = stat->endTime - proc->arrivalTime + 1;
		float waitingTime = turnaround - runTime;
		unsigned char priority = proc->priority;
		avg_response_time += responseTime;
		avg_wait_time += waitingTime;
		avg_turnaround += turnaround;
		process_count++;
		printf("%16c|%14.1f|%12.1f|%10.1f|%10.1f|%15.1f|%11.1f| %17.1f|%10u|\n",proc->pid,arrivalTime, stat->startTime, stat->endTime, runTime, responseTime, waitingTime, turnaround,priority);
		ptr = ptr->next;
	}
    
	avg.avg_throughput = process_count;
	
    if (process_count == 0) process_count = 1;

	avg_response_time /= process_count;
	avg_wait_time /= process_count;
	avg_turnaround /= process_count;

	printf("-----------------------------------------------------------------------------------------------------------------------------\n");
	printf("%16s|%14.1f|%12.1f|%10.1f|%10.1f|%15.1f|%11.1f| %17.1f|\n", "AVERAGE", 0.0, 0.0, 0.0, 0.0, avg_response_time, avg_wait_time, avg_turnaround);
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");

	avg.avg_response_time = avg_response_time;
	avg.avg_wait_time = avg_wait_time;
	avg.avg_turnaround = avg_turnaround;

	return avg;
}

