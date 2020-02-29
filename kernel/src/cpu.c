#include <string.h>
#include "ram.h"
#include "cpu.h"
#include "shell.h"

void run_cpu(struct CPU *cpu, int end) {
	
	if (cpu->IP >= end) {
		cpu->IP = -1;
		return;
	}

	for (int i = 0; i < cpu->quanta; i++) {
		if ((cpu->IP) + i >= end) break;
		cpu->IR[i] = memGet(cpu->IP + i);
	}

	for (int j = 0; j < cpu->quanta; j++) {
		if (cpu->IP >= end) {
			cpu->IP = -1;
			break;
		}
		char *command = cpu->IR[j];
		parse(command, 1);
		cpu->IP++;
	}
}
