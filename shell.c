#include "mySHfunc.h"

int shouldRun = 1;

int main(int argc, char *argv[])
{
    char args[MAX_LINE/2 + 1], prompt[4] = "seq";
    int styleType = 0, styleChange;

    if(argc == 2) // BATCH MODE
    {
        FILE *file;

        file = fopen(argv[1], "r");

        if(file == NULL) // tratamento de erro
        {
            printf("File not found.\n");
            fclose(file);
            exit(1);
        }
        
        if(!hasExit(file))
        {
            printf("File has no exit command.\n");
            fclose(file);
            exit(1);
        }

        rewind(file);

        char line[max];

        while(fgets(line, max, file) != NULL)
        {
            styleChange = 0;
            if(shouldRun)
            {
                for(int i = 0; i < strlen(line); i++)
                {
                    if(line[i] == '\n') line[i] = '\0';
                }

                if(isEmpty(line)) continue; // tratamento de erro
                
                char *temp[MAX_LINE/2 + 1];
                int contCommands = 0;
                
                char *aux = strtok(line, ";");
                while(aux != NULL)
                {
                    if(!isEmpty(aux))
                    {
                    temp[contCommands] = (char *)malloc(strlen(aux)+1);
                    removeExcessSpaces(aux, temp[contCommands]);
                    contCommands++;
                    }
                    aux = strtok(NULL, ";");
                }
                temp[contCommands] = '\0';
                
                for(int j=0; j<contCommands; j++)
                {
                    if(strcmp(temp[j], "exit") == 0)
                    {
                        shouldRun = 0;
                        break;
                    }
                    else if(strcmp(temp[j], "style parallel") == 0)
                    {
                        styleType = 1;
                        styleChange = 1;
                    }
                    else if(strcmp(temp[j], "style sequential") == 0)
                    {
                        styleType = 0;
                        styleChange = 1;
                    }
                }

                if(shouldRun == 1)
                {
                    if(styleChange == 0)
                    {
                        if(styleType == 0) printf("Style type: Sequential.\n");
                        else if(styleType == 1) printf("Style type: Parallel.\n");
                        
                        pid_t process;
                        int status;

                        process = fork();

                        if(process < 0)
                        {
                            perror("pid");
                            exit(1);
                        }
                        else if(process == 0)
                        {
                            if(styleType == 0)
                            {
                                for(int i=0; i<contCommands; i++) executeSequential(temp[i]);
                            }
                            else if(styleType == 1) executeParallel(temp, contCommands);

                            exit(EXIT_SUCCESS);
                        }
                        else
                        {
                            waitpid(process, &status, 0);
                            if (!WIFEXITED(status))
                            {
                                perror("Error in Child Process.\n");
                                printf("Child process did not exit normally.\n");
                            }
                        }
                    }

                    for(int j=0; j < contCommands; j++) free(temp[j]);
                }
                else exit(0);

                memset(line, 0, max);
            }
        }

        fclose(file);
    }
    else
    {
        while(shouldRun == 1)
        {
            styleChange = 0;

            printf("abxa %s> ", prompt);
            if(fgets(args, sizeof(args), stdin) == NULL)
            {
                printf("Error reading commands.\n");
                exit(1);
            }

            if(strcmp(args, "\n") == 0) continue; // tratamento de erro

            for(int i = 0; i < strlen(args); i++)
            {
                if(args[i] == '\n') args[i] = '\0';
            }

            if(isEmpty(args)) continue; // tratamento de erro

            char *temp[MAX_LINE/2 + 1];
            int contCommands = 0;

            char *aux = strtok(args, ";");
            while(aux != NULL)
            {
                if(!isEmpty(aux))
                {
                    temp[contCommands] = (char *)malloc(strlen(aux)+1);
                    removeExcessSpaces(aux, temp[contCommands]);
                    contCommands++;
                }
                aux = strtok(NULL, ";");
            }
            temp[contCommands] = '\0';

            for(int j=0; j<contCommands; j++)
            {
                if(strcmp(temp[j], "exit") == 0)
                {
                    shouldRun = 0;
                    break;
                }
                else if(strcmp(temp[j], "style parallel") == 0)
                {
                    styleType = 1;
                    styleChange = 1;
                    strcpy(prompt, "par");
                }
                else if(strcmp(temp[j], "style sequential") == 0)
                {
                    styleType = 0;
                    styleChange = 1;
                    strcpy(prompt, "seq");
                }
            }

            if(shouldRun == 1)
            {
                if(styleChange == 0)
                {
                    pid_t process;
                    int status;

                    process = fork();

                    if(process < 0)
                    {
                        perror("pid");
                        exit(1);
                    }
                    else if(process == 0)
                    {
                        if(styleType == 0)
                        {
                            for(int i=0; i<contCommands; i++) executeSequential(temp[i]);
                        }
                        else if(styleType == 1) executeParallel(temp, contCommands);

                        exit(EXIT_SUCCESS);
                    }
                    else
                    {
                        waitpid(process, &status, 0);
                        if (!WIFEXITED(status))
                        {
                            perror("Error in Child Process.\n");
                            printf("Child process did not exit normally.\n");
                        }
                    }
                }
                for(int j=0; j < contCommands; j++) free(temp[j]);
            }
            else exit(0);
        }
    }
    return 0;
}