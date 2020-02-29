#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "shellmemory.h"
#include "kernel.h"

extern int BUFFER_SIZE;

enum Error {
    UNKNOWN_COMMAND,
    SCRIPT_NOT_FOUND, 
    VARIABLE_DOES_NOT_EXIST,
	INVALID_ARGS
};

struct Command {
    char* name;
    char* description;
};

struct Command commands[] = {
    { "help", "                          Displays all the commands" },
    { "quit", "                          Exits / terminates the shell with \"Bye!\"" },
    { "set VAR STRING", "                Assigns a value to shell memory" },
    { "print VAR", "                     Displays the STRING assigned to VAR" },
    { "run SCRIPT.TXT", "                Executes the file SCRIPT.TXT" },
	{ "exec p1 p2 p3", "                 Executes scripts concurrently" }
};

void error(enum Error e) {
    switch(e) {
        case UNKNOWN_COMMAND:
            printf("Unknown command\n");
            break;
        case SCRIPT_NOT_FOUND:
            printf("Script not found\n");
            break;
        case VARIABLE_DOES_NOT_EXIST:
            printf("Variable does not exist\n");
            break;
		case INVALID_ARGS:
			printf("Invalid arguments\n");
        default:
            break;
    }
}


int script(char *scriptName) {
    int errCode = 0;
    char line[BUFFER_SIZE];
    FILE *p;
    
    if ((p = fopen(scriptName, "r")) == NULL) {
        error(SCRIPT_NOT_FOUND);
        return 1;
    }

    fgets(line, BUFFER_SIZE - 1, p);

    while (!feof(p)) {
        errCode = parse(line, 0);

        if (errCode != 0) {
            fclose(p);
            return errCode;
        }

        fgets(line, BUFFER_SIZE - 1, p);
    }

    fclose(p);
    return errCode;
}

int help() {
    for (int i = 0; i < 6; i++) {
        struct Command command = commands[i];
        printf("%s%s\n", command.name, command.description);
    }
    return 0;
}

void quit() {
    printf("Bye!\n");
    exit(0);
}

int set(char *args[], int numArgs) {

	if (numArgs < 3) {
		error(INVALID_ARGS);
		return 1;
	}
    
	char value[1024];
    strcpy(value, args[2]);

    for (int i = 3; i < numArgs; i++) {
		strcat(value, " ");
        strcat(value, args[i]);
    }

    setMem(args[1], value);
	return 0;
}

int exec(char *args[], int numArgs) {
	for (int i = 1; i < numArgs; i++) {
		myInit(args[i]);
	}
	scheduler();
    return 1;
}

int print(char *args[]) {
    char* value = getMem(args[1]);

    if (value == NULL) error(VARIABLE_DOES_NOT_EXIST);
    else printf("%s\n", value);
    
    return 0;
}

int run(char *args[]) {
    return script(args[1]);
}

int interpreter(char *tokens[], int n) { 

    int errorCode = 0;
    
    char *commandName = tokens[0];

    if (strcmp(commandName, "help") == 0)          	errorCode = help();
    else if (strcmp(commandName, "quit") == 0)     	quit();
    else if (strcmp(commandName, "set") == 0)      	errorCode = set(tokens, n);
    else if (strcmp(commandName, "run") == 0)      	errorCode = run(tokens);
    else if (strcmp(commandName, "print") == 0)    	errorCode = print(tokens);
    else if (strcmp(commandName, "exec") == 0)	   	errorCode = exec(tokens, n);
	else {
        error(UNKNOWN_COMMAND);
    }

    for (int i = 0; i < n; i++) free(tokens[i]);
    
    return errorCode;
}
