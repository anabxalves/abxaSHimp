#include "mySHfunc.h"
#define MAX 100

int isExit(FILE *file)
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

        aux = strtok(line, ";");
        while(aux != NULL)
        {
            char temp[MAX];
            int flagLastSpace = 0, j=0;
            for(int i = 0; i < strlen(aux); i++)
            {
                
                if(aux[i] == ' ')
                {
                    if(i != 0 && i != (strlen(aux)-1))
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
                
                temp[j] = aux[i];
                j++;
            }
            temp[j] = '\0';
            printf("%s\n", temp);
            memset(temp, 0, 0);

            aux = strtok(NULL, ";");
        }
        
        if(strcmp(line, "exit") == 0) break;
    }
}
