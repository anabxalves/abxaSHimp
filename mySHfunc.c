#include "mySHfunc.h"

int hasExit(FILE *file)
{
    char line[max];

    while(fgets(line, max, file) != NULL)
    {
        for(int i = 0; i < strlen(line); i++)
        {
            if(line[i] == '\n') line[i] = 0;
        }

        char flagExit[max];
        char *aux = strtok(line, ";");
        while(aux != NULL)
        {
            removeExcessSpaces(aux, flagExit);

            if(strcmp(flagExit, "exit") == 0) return 1;
            
            aux = strtok(NULL, ";");
        }
    }
    return 0;
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

void removeExcessSpaces(char *origin, char* result)
{
    char temp[MAX_LINE/2 +1];
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

void *commandRunner(void *command)
{
    system(command);
    pthread_exit(0);
}

void executeList(char** list, int size)
{
    char** aux = list;

    for(int i=0; i< size; i++)
    {
        printf("- Executing '%s' -\n", aux[i]);
        system(aux[i]);
    }
}

void executeSequential(char *list)
{
    char *arg = strdup(list);
    int qtdCommands = 0;
    char *commands[MAX_LINE/2 + 1];
    qtdCommands = getCommandArg(list, commands, (MAX_LINE/2 + 1));

    pid_t child = fork();
    int status;

    if (child < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (child == 0)
    {
        printf("- Executing '%s' -\n", arg);
        
        execvp(commands[0], commands);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else
    {
        waitpid(child, &status, 0);

        if (!WIFEXITED(status)) {
            perror("Error in Child Process.\n");
            printf("Child process did not exit normally.\n");
        }
    }
    memset(commands, 0, qtdCommands);
}

int getCommandArg(char *command, char *parts[], int maxParts)
{
    int qtdArgs = 0;
    char *aux = strtok(command, " ");

    while (aux != NULL && qtdArgs < maxParts - 1)
    {
        parts[qtdArgs] = aux;
        qtdArgs++;
        aux = strtok(NULL, " ");
    }
    parts[qtdArgs] = NULL; // LAST ELEMENT NULL

    return qtdArgs;
}

void executeParallel(char *parsed[], int contCommands)
{
    pthread_t commandThreads[contCommands];
    int commandControl[contCommands], cont = 0, shouldExit = 0;

    for(int i=0; i < contCommands; i++)
    {
        commandControl[i] = pthread_create(&commandThreads[i], NULL, commandRunner, parsed[i]);
        
        if(commandControl[i] != 0)
        {
            perror("pthread_create error");
            exit(EXIT_FAILURE);
        }
        cont++;
    }
    
    for (int i = 0; i < cont; i++)
    {
        printf("- Executing '%s' -\n", parsed[i]);
        if (pthread_join(commandThreads[i], NULL) != 0)
        {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }
}