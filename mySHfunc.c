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
    char temp[MAX_LINE];
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
    if(strcmp(command, "No commands") == 0) printf("No commands\n");
    else system(command);

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
    char *commands[MAX_LINE];
    qtdCommands = getCommandArg(list, commands, (MAX_LINE));

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
    int commandControl[contCommands], cont = 0;

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
        if(strcmp(parsed[i], "No commands") != 0) printf("- Executing '%s' -\n", parsed[i]);

        if (pthread_join(commandThreads[i], NULL) != 0)
        {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }
}

int hasChar(char *command, char comp)
{
    for(int i = 0; i < strlen(command); i++)
    {
        if(command[i] == comp) return 1;
    }
    return 0;
}

void executePipe(char **commands)
{
    pid_t child_pid1, child_pid2;
    int pipe_fd[2], status1, status2;

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    child_pid1 = fork();

    if (child_pid1 < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if(child_pid1 == 0)
    {
        char *pipeParsed1[MAX_LINE];
        int qtdCommands1 = getCommandArg(commands[0], pipeParsed1, (MAX_LINE));

        close(pipe_fd[0]);

        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);

        execvp(pipeParsed1[0], pipeParsed1);
        perror("execvp");
        exit(EXIT_FAILURE);

        memset(pipeParsed1, 0, qtdCommands1);
    }

    child_pid2 = fork();
    if (child_pid2 < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if(child_pid2 == 0)
    {
        char *pipeParsed2[MAX_LINE];
        int qtdCommands2 = getCommandArg(commands[1], pipeParsed2, (MAX_LINE));

        close(pipe_fd[1]);

        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]);

        execvp(pipeParsed2[0], pipeParsed2);
        perror("execvp");
        exit(EXIT_FAILURE);
        
        memset(pipeParsed2, 0, qtdCommands2);
    }

    close(pipe_fd[0]);
    close(pipe_fd[1]);

    waitpid(child_pid1, &status1, 0);
    waitpid(child_pid2, &status2, 0);

    if (!WIFEXITED(status1) || !WIFEXITED(status2)) {
        perror("Error in Grandchild Process.\n");
        printf("Grandchild process did not exit normally.\n");
    }
}

void myTrimSpaces(char *command)
{
    char temp[MAX_LINE];
    int j = 0;

    for(int i = 0; i < strlen(command); i++)
    {
        if(command[i] == ' ' && i == 0) continue;
        else if(command[i] == ' ' && i == strlen(command) - 1) continue;
        else
        {
            temp[j] = command[i];
            j++;
        }
    }
    temp[j] = '\0';
    strcpy(command, temp);
}