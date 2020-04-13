#include <stdio.h>
#include <stdlib.h>
#include "pcb.h"

struct PCB *makePCB(int page_start, int page_max) {
	struct PCB *pcb = (struct PCB *)malloc(sizeof(struct PCB));
	pcb->PC = 0;
	pcb->PC_page = page_start;
	pcb->PC_offset = 0;
	pcb->pages_max = page_max;
	pcb->next = NULL;
	pcb->page_start = page_start;

	for (int i = 0; i < 10; i++) {
		pcb->pageTable[i] = -1;
	}

	return pcb;
}
