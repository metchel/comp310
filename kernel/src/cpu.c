#include <string.h>
#include "ram.h"
#include "cpu.h"
#include "shell.h"

int run_cpu(struct CPU *cpu) {
	
	// PAGE FAULT
	if (cpu->offset >= 4) {
		return -1;
	}

	for (int i = 0; i < cpu->quanta; i++) {
		if ((cpu->offset) + i >= 4) break;
		cpu->IR[i] = memGet(cpu->IP + cpu->offset + i);
	}

	for (int j = 0; j < cpu->quanta; j++) {
		// PAGE FAULT
		if (cpu->offset >= 4) {
			return -1;
		}
		char *command = cpu->IR[j];

		// Only run commands that aren't dumb.
		parse(command, 0);
		cpu->offset++;
	}

	return 0;
}
