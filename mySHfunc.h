#ifndef MYSHFUNC_H
#define MYSHFUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#define MAX_LINE 80
#define max 100

int hasExit(FILE *file); // confere se existe (1) ou não (0) o comando exit 
void executeList(char** list, int size);
int isEmpty(char *line);
void removeExcessSpaces(char *origin, char* result);
void *commandRunner(void *command);
void executeSequential(char *list);
int getCommandArg(char *command, char *parts[], int maxParts);
void executeParallel(char *parsed[], int contCommands);

#endif