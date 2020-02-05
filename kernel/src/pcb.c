#include <stdio.h>
#include <stdlib.h>
#include "pcb.h"

struct PCB *makePCB(int start, int end) {
	struct PCB *pcb = (struct PCB *)malloc(sizeof(struct PCB));
	pcb->PC = start;
	pcb->start = start;
	pcb->end = end;
	pcb->next = NULL;
	return pcb;
}
