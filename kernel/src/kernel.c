#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "pcb.h"
#include "cpu.h"
#include "ram.h"

struct CPU *cpu;
struct PCB *head, *tail;

void createQueue() {
	head = tail = NULL;
}

void enQueue(struct PCB *pcb) {
	pcb->next = NULL;
	if (tail == NULL) {
		head = pcb;
		tail = pcb;
		return;
	}
	tail->next = pcb;
	tail = tail->next;
}

void deQueue() {
	if (head == NULL)
		return;

	struct PCB *temp = head;
	
	head = head->next;

	if (head == NULL) 
		tail = NULL;
}

void printPCB(struct PCB *pcb) {
	printf(" { PC: %d, start: %d, end: %d } ", pcb->PC, pcb->start, pcb->end);
}

void printQueue() {
	struct PCB *pcb = head;
	printf("Q: [");
	while (pcb != NULL) {
		printPCB(pcb);
		pcb = pcb->next;
	}
	
	printf("]\n");
}

void printCPU(struct CPU *cpu) {
	printf("CPU: { IP: %d, IR: [ \"%s\", \"%s\" ]\n", cpu->IP, cpu->IR[0], cpu->IR[1]);
}

void myInit(char *filename) {
	FILE *p;

	if ((p = fopen(filename, "r")) == NULL) {
		printf("File not found.");
	}

	int start, end;
	addToRAM(p, &start, &end);
	struct PCB *pcb = makePCB(start, end);
	enQueue(pcb);
}

void scheduler() {
	
	while (head != NULL) {	
		
		struct PCB *temp = head;
		deQueue();
		cpu->IP = temp->PC;
		run_cpu(cpu, temp->end);
		
		if (cpu->IP < 0) {
			removeFromRAM(temp->start, temp->end);
			free(temp);
		} else {
			temp->PC = cpu->IP;
			enQueue(temp);
		}
	}
}

int main() {

	createQueue();
	cpu = (struct CPU*)malloc(sizeof(struct CPU));
	cpu->quanta = 2;

	int errorCode = 1;
	while ((errorCode = shellUI()) > 0) {}
}
