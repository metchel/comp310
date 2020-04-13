#ifndef PCB_H
#define PCB_H

struct PCB {
	int PC;
	int PC_page;
	int PC_offset;
	int pages_max;
	int page_start;
	int pageTable[10];
	struct PCB *next;
};

struct PCB* makePCB(int page_start, int pages_max);

#endif
