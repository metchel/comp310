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
		char *instruction = memGet(cpu->IP + i);
		cpu->IR[i] = instruction;
	}

	for (int j = 0; j < cpu->quanta; j++) {
		if (cpu->IP >= end) {
			cpu->IP = -1;
			break;
		}
		char *command = cpu->IR[j];
		parse(command);
		cpu->IP++;
	}
}
