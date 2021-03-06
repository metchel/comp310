#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"

#define WELCOME "Welcome to the Etch-shell!"
#define VERSION "Version 1.1 Created February 2020"
#define PROMPT  "$"

int BUFFER_SIZE = 1024;
size_t BUFFER_SIZE_T = 1024;
enum boolean { FALSE, TRUE };

int parse(char input[], int flag) {
    if (input == NULL || input[0] == EOF) {
        return 1;
    }

    char *tokens[32];
    char tmp[256];

    int i, j; int k = 0;

    for (i = 0; input[i] == ' ' && i < BUFFER_SIZE; i++);

    while (input[i] != '\0' && input[i] != EOF &&  i < BUFFER_SIZE) {
        
        for (j = 0; input[i] != '\0' && input[i] != ' ' && input[i] != '\r'  && i < BUFFER_SIZE && input[i] != '\n'; i++, j++) {
            tmp[j] = input[i];
        }

        tmp[j] = '\0';
        tokens[k] = strdup(tmp);

        i++; k++;
    }

	if (flag) k--;

    return interpreter(tokens, k);
}

int shellUI() {

    char *input = NULL;
    int errorCode = 0;

    printf("%s\n%s\n", WELCOME, VERSION);
    
    ssize_t linelen;

    printf("%s ", PROMPT);

    while (TRUE) {
        
        while ((linelen = getline(&input, &BUFFER_SIZE_T - 1, stdin)) != -1) {
			errorCode = parse(input, 0);
            if (errorCode == -1) {
                return errorCode;
            }
            break;
        }

        if (linelen == -1) {
            return 0;
        }

        printf("%s ", PROMPT);
    }
}
