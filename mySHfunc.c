#include "mySHfunc.h"

int hasExit(FILE *file)
{
    char line[MAX];

    while(fgets(line, MAX, file) != NULL)
    {
        for(int i = 0; i < strlen(line); i++)
        {
            if(line[i] == '\n') line[i] = 0;
        }

        char flagExit[MAX];
        char *aux = strtok(line, ";");
        while(aux != NULL)
        {
            getParsed(aux, flagExit);

            if(strcmp(flagExit, "exit") == 0) return 1;
            
            aux = strtok(NULL, ";");
        }
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

int execution(int style, char *commands[MAX], int qtdCommands)
{
    pid_t pai;

    pai = fork();

    if(pai < 0)
    {
        fprintf(stderr, "Fork Failed.");
		return 1;
    }
    else if(pai == 0) // processo filho
    {
        if(style == 0) // sequential
        {
            char elements[MAX][MAX];
            int qtdElements[MAX];
           
            for(int i=0; i< qtdCommands; i++) qtdElements[i] = getCommandArg(commands, elements[i]);

            for(int j=0; j< qtdCommands; j++)
            {
                execvp(elements[j], qtdElements[j]);
            }
        }
        else if(style == 1) // parallel
        {
            /* comando system ao invés do exec */
            
        }

    }
    else // pai
    {
        wait(NULL);
    }

    return 1;
}

int getCommandArg(char *origin, char *result[MAX])
{
    char *temp, ch = '\\';
    int j=0;

    strcpy(temp, origin);
    strcpy(temp, strrchr(temp, ch)); // tratar casos em que o path é informado

    temp = strtok(temp, " ");
    while(temp != NULL)
    {
        strcpy(result[j], temp);
        j++;
        temp = strtok(NULL, " ");
    }

    return j;
}