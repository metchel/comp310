#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int BUFFER_SIZE;

char *ram[1000];

int next_ram_cell_index() {
	int i = 0;
	while (ram[i] != NULL) {	
		i++;
	}
	return i;
}

void addToRAM(FILE *p, int *start, int *end) {
    
	int next_cell = next_ram_cell_index();
	int start_cell = next_cell;

    int errCode = 0;
    char line[BUFFER_SIZE];

	fgets(line, BUFFER_SIZE - 1, p);
    
    while (!feof(p) && next_cell < 999) {
		if (ram[next_cell] != NULL) {
			fclose(p);
			exit(1);
		}
		ram[next_cell] = strdup(line); 
		next_cell++;
		fgets(line, BUFFER_SIZE - 1, p);	
	}

	int i = 0;
	while ( i < 1000 && ram[i] != NULL) {
		printf("%s", ram[i]);
		i++;
	}
    fclose(p);
}
