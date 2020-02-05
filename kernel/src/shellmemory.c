#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct MEM {
    char* key;
    char* value;
};

struct MEM shellMemory[1024];

void setMem(char *key, char *value) {
	for (int i = 0; i < 1024; i++) {
        if (shellMemory[i].key == NULL) {
            shellMemory[i].key = strdup(key);
            shellMemory[i].value = strdup(value);
            break;
        }
        
        if (strcmp(shellMemory[i].key, key) == 0) {
            shellMemory[i].value = strdup(value);
            break;
        }

    }
}

char *getMem(char *key) {
    
    for (int i = 0; i < 1024; i++) {
        if (shellMemory[i].key == NULL) {
            break;
        }
        
        if (strcmp(shellMemory[i].key, key) == 0) {
            return shellMemory[i].value;
        }
    }

    return NULL;
}
