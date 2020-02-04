#include <stdlib.h>
#include "pcb.h"

struct PCB *makePCB(int start, int end) {
	struct PCB *pcb = (struct PCB*)malloc(sizeof(struct PCB));
	pcb->PC = 0;
	pcb->start = start;
	pcb->end = end;

	return pcb;
}
