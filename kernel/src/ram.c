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

void printRAM() {
	for (int i = 0; i < 999; i++) printf("|%s", ram[i]);
	printf("\n");
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
		line[strcspn(line, "\r\n")] = 0;
		ram[next_cell] = strdup(line); 
		next_cell++;
		fgets(line, BUFFER_SIZE - 1, p);	
	}
	
	*start = start_cell;
	*end = next_cell;

    fclose(p);
}

void removeFromRAM(int start, int end) {
	if (start < 0 || end > 999) {
		printf("Invalid RAM location");
		return;		
	}	

	while (start <= end) {
		ram[start] = NULL;
		start++;
	}
}

char *memGet(int cell) {
	if (cell < 0 || cell > 999) return NULL;
	return strdup(ram[cell]);
}
