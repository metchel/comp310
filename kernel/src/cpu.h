#ifndef CPU_H
#define CPU_H

struct CPU {
	int IP;
	int offset;
	char *IR[2];
	int quanta;
};

int run_cpu(struct CPU *cpu);

#endif
