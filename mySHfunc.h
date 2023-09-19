#ifndef MYSHFUNC_H
#define MYSHFUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#define MAX 100

int hasExit(FILE *file); // confere se existe (1) ou não (0) o comando exit 
void printFile(FILE *file); // imprime os comandos lidos no arquivo, até o exit
void getParsed(char *origin, char *result); // separa os comandos que estão na mesma linha
int isEmpty(char *line);

#endif