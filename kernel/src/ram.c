#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int BUFFER_SIZE;

char *ram[1000];

int next_ram_cell_index() {
	int i = 0;
	while (ram[i] == NULL) {
		i++;
	}
	return i;
}

void addToRAM(FILE *p, int *start, int *end) {
    int next_cell = next_ram_cell_index();
	int start_cell = next_cell;

    int errCode = 0;
    char line[BUFFER_SIZE];
    
    while (!feof(p)) {
		if (ram[next_cell] != NULL) {
			fclose(p);
			exit(1);
		}
		fgets(line, BUFFER_SIZE - 1, p);
		ram[next_cell] = strdup(line); 
		next_cell++;
    }

    fclose(p);
}
