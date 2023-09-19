#include "mySHfunc.h"

int hasExit(FILE *file)
{
    char aux[MAX];

    while(fgets(aux, MAX, file) != NULL)
    {
        for(int i = 0; i < strlen(aux); i++)
        {
            if(aux[i] == '\n') aux[i] = 0;
        }

        if(strcmp(aux, "exit") == 0) return 1;
        /* perguntar se exit vai ter ; ou não */
    }
    return 0;
}

void printFile(FILE *file)
{
    char line[MAX], *aux;

    while(fgets(line, MAX, file) != NULL)
    {
        for(int i = 0; i < strlen(line); i++)
        {
            if(line[i] == '\n') line[i] = 0;
        }
        char commands[MAX];
        int j=0;
        aux = strtok(line, ";");
        while(aux != NULL)
        {
            getParsed(aux, &commands[j]);

            if(strcmp(&commands[j], "exit") == 0) break;
            else
            {
                printf("Executing: %s\n", &commands[j]);
                j++;
                aux = strtok(NULL, ";");
            }
        }
        
        if(strcmp(line, "exit") == 0) break;
    }
}

void getParsed(char *origin, char *result)
{
    char temp[MAX];
    int flagLastSpace = 0, j=0;
    
    for(int i = 0; i < strlen(origin); i++)
    {
        if(origin[i] == ' ')
        {
            if(i != 0 && i != (strlen(origin)-1))
            {
                if(flagLastSpace == 1) continue;
                else flagLastSpace = 1;
            }
            else
            {
                flagLastSpace = 1;
                continue;
            }
        }
        else flagLastSpace = 0;
        
        temp[j] = origin[i];
        j++;
    }
    temp[j] = '\0';
    strcpy(result, temp);
}

int isEmpty(char *line)
{
    int count = 0;

    for(int i=0; i<strlen(line); i++)
    {
        if(line[i] > 32) count++;
    }

    if(count > 0) return 0;
    else return 1;
}