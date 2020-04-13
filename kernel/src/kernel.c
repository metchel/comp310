#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "pcb.h"
#include "cpu.h"
#include "ram.h"
#include "memorymanager.h"

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


void printQueue() {
	struct PCB *pcb = head;
	printf("Q: [");
	while (pcb != NULL) {
		printf("{ PC: %d, PC_offset: %d, PC_page: %d }", pcb->PC, pcb->PC_offset, pcb->PC_page);
		pcb = pcb->next;
	}
	
	printf("]\n");
}

void printCPU(struct CPU *cpu) {
	printf("CPU: { IP: %d, offset: %d, IR: [ \"%s\", \"%s\" ]\n", cpu->IP, cpu->offset, cpu->IR[0], cpu->IR[1]);
}

void myInit(char *filename) {
	FILE *p;

	if ((p = fopen(filename, "r")) == NULL) {
		printf("File not found.");
	}

	fclose(p);
}

int pageFault(struct PCB* pcb) {
	// Go to the next page
	
	freeFrames(pcb->pageTable[pcb->PC_page]);
	pcb->PC_page++;

	if (pcb->PC_page >= pcb->pages_max) {
		// The program is done. That was the last page.
		return 0;
	}

	if (pcb->pageTable[pcb->PC_page] >= 0) {
		// then this page is already in a frame of RAM.
		// set the counter to the first line of the frame
		pcb->PC = pcb->pageTable[pcb->PC_page] * 4;
		pcb->PC_offset = 0;
	} else {
		// we need to put the next page into a frame in RAM.
		// find the next frame
		int availableFrame = findFrame();
		int frame = -1;
		int victim = -1;
		if (availableFrame >= 0) {
			frame = availableFrame;
		} else {
			frame = findVictim(pcb);
		}
		int pageNumber = pcb->PC_page;
		char filename[64];
		sprintf(filename, "BackingStore/PAGE_%d", pageNumber);
		FILE *f = fopen(filename, "r");

		loadPage(pageNumber, f, availableFrame);
		updatePageTable(pcb, pageNumber, availableFrame, victim);
		
		pcb->PC = pcb->pageTable[pcb->PC_page] * 4;
		pcb->PC_offset = 0;
	}

	return 1;
}

void scheduler() {
	
	while (head != NULL) {	
		struct PCB *temp = head;
		deQueue();
		cpu->IP = temp->PC;
		cpu->offset = temp->PC_offset;

		int errorCode = run_cpu(cpu);
		
		// PAGE FAULT
		if (errorCode < 0) {
			int pageFaultErrCode = pageFault(temp);
			if (pageFaultErrCode == 0) {
				// the program finished.
				for (int i = 0; i < 10; i++) freeFrames(temp->pageTable[i]);				
				free(temp);
			} else {
				// the program hasn't finished.
				enQueue(temp);
			} 
		} else {
			temp->PC = cpu->IP;
			temp->PC_offset = cpu->offset;
			enQueue(temp);
		}
	}
}

void boot() {
	system("rm -rf BackingStore");
	system("mkdir BackingStore");
}

int kernel() {
	createQueue();
	cpu = (struct CPU*)malloc(sizeof(struct CPU));
	cpu->quanta = 2;

	initRAM();

	int errorCode = 0;
	while ((errorCode = shellUI()) > 0) {}
	return errorCode;
}

int main() {
	boot();

	int errorCode = 0;
	errorCode = kernel();
	return errorCode;
}
