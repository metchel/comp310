#include <stdio.h>
#include <stdlib.h>
#include "shell.h"
#include "pcb.h"
#include "cpu.h"
#include "ram.h"

struct ReadyQueue *readyQueue;

struct Node {
	struct PCB *pcb;
	struct Node *next;
};

struct ReadyQueue {
	struct Node *head, *tail;
};

struct Node *newNode(struct PCB *pcb) {
	struct Node *temp = (struct Node*)malloc(sizeof(struct Node));
	temp->pcb = pcb;
	temp->next = NULL;
	return temp;
}

struct ReadyQueue *createQueue() {
	struct ReadyQueue *q = (struct ReadyQueue*)malloc(sizeof(struct ReadyQueue));
	q->head = q->tail = NULL;
	return q;
}

void enQueue(struct ReadyQueue *q, struct PCB *pcb) {
	struct Node *temp = newNode(pcb);
	
	if (q->head == NULL) {
		q->head = q->tail = temp;
	}

	q->tail->next = temp;
	q->tail = temp;
}

void deQueue(struct ReadyQueue *q) {
	if (q->head == NULL) {
		return;
	}

	struct Node *temp = q->head;

	q->head = q->head->next;

	if (q->head == NULL) {
		q->tail = NULL;
	}

	free(temp);
}

void addToReadyQueue(struct ReadyQueue *q, struct PCB *pcb) {	
	enQueue(readyQueue, pcb);
}

void myInit(char *filename) {
	FILE *p;

	if ((p = fopen(filename, "r")) == NULL) {
		printf("File not found.");
	}

	int start, end;
	addToRAM(p, &start, &end);
	struct PCB *pcb = makePCB(start, end);
	addToReadyQueue(readyQueue, pcb);
}

void scheduler() {
}

int main() {

	readyQueue = createQueue();
	int errorCode = 1;
	while ((errorCode = shellUI()) > 0) {
		
	}
}
