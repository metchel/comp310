#ifndef RAM_H
#define RAM_H

#include <stdio.h>

void printRAM();
void addToRAM(FILE *p, int *start, int *end);
void removeFromRAM(int start, int end);
char *memGet(int cell);
void memSet(int cell, char* value);
void initRAM();
char* getRAM(int cell);
#endif
