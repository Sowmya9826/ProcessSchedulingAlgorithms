#ifndef _COMMON_h_
#define _COMMON_h_

struct node_s {
	struct node_s *next;
	struct node_s *prev;
	void *data;
};

typedef struct node_s node;

struct linked_list_s {
	node *head;
	node *tail;
	int size;
};

typedef struct linked_list_s linked_list;

node *createNode(void *data);
linked_list *createLinkedList();
void addNode(linked_list *ll, void *data);

// Function to remove a node from a linked list
void removeNode(linked_list *ll, void *data);

// Function to swap two nodes in a linked list
void swapNodes(node *a, node *b);

// Function to add a node after a specified node in a linked list
void addAfter(linked_list *ll, node *after_node, void *data);

// Function to sort a linked list using a comparison function
void sort(linked_list *ll, int (*cmp)(void *data1, void *data2));

// Queue Implementation
typedef struct linked_list_s queue;

// Function to create a queue
queue *createQueue();

// Function to add an element to the queue
void enqueue(queue *q, void *data);

// Function to remove an element from the queue
void *dequeue(queue *q);

#endif

