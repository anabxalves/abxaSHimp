#include "mySHfunc.h"

int main(int argc, char *argv[])
{
    if(argc == 2) /* modo batch */
    {
        FILE *file;

        file = fopen(argv[1], "r");

        if(file == NULL) // tratamento de erro
        {
            printf("File not found.\n");
            fclose(file);
            exit(1);
        }

        if(hasExit(file))
        {
            rewind(file);

            printf("Exit command found!\n");
            printFile(file);
            fclose(file);
            exit(0);
        }
        else // tratamento de erro
        {
            printf("File has no exit command.\n");
            fclose(file);
            exit(1);
        }
    }
    
    /* modo interativo */
    
    int isExited = 0;
    int style = 0; // 0 = sequential / 1 = parallel
    char prompt[4] = "seq";

    while(!isExited)
    {
        printf("abxa %s> ", prompt);
        
        char line[MAX], *aux;
        /* usar strrchr(); */
        
        if(fgets(line, MAX, stdin) == NULL)
        {
            printf("Error to read input.\n");
            exit(1);
        }

        if (strcmp(line, "\n") == 0) continue; // tratamento de erro

        for(int i = 0; i < strlen(line); i++)
        {
            if(line[i] == '\n') line[i] = 0;
        }
        if(isEmpty(line)) continue; // tratamento de erro
        else
        {
            char commands[MAX][MAX];
            int j=0; //serve como cont do array commands (j+1)
            aux = strtok(line, ";");
            while(aux != NULL)
            {
                getParsed(aux, commands[j]);

                if(strcmp(commands[j], "exit") == 0)
                {
                    isExited = 1;
                    break;
                }
                
                if(strcmp(commands[j], "style parallel") == 0)
                {
                    style = 1;
                    strcpy(prompt, "par");
                }
                else if(strcmp(commands[j], "style sequential") == 0)
                {
                    style = 0;
                    strcpy(prompt, "seq");
                }
                else if(!isEmpty(commands[j])) j++;

                aux = strtok(NULL, ";");
            }
            strcpy(commands[j], "\0"); 

            for (int i = 0; i < j; i++) printf("Executing: %s\n", commands[i]);

        }
        
        /*
        usar o getCommandArg para ver quantos elementos o comando tem 
    
        perguntar pra erico se vai ter troca de estilo no meio das prompts
            caso sim, fazer n arrays para cada troca de estilo

        perguntar se batch pode ser paralelo
        */
    }

    return 0;
}