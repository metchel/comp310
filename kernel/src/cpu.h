#ifndef CPU_H
#define CPU_H

struct CPU {
	int IP;
	char *IR[2];
	int quanta;
};

void run_cpu(struct CPU *cpu, int end);

#endif
