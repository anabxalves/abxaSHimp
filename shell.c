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

    while(!isExited)
    {
        printf("abxa seq> ");
        
        char line[MAX], *aux;
        
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
            char commands[MAX];
            int j=0;
            aux = strtok(line, ";");
            while(aux != NULL)
            {
                getParsed(aux, &commands[j]);
                if(strcmp(&commands[j], "exit") == 0)
                {
                    isExited = 1;
                    exit(0);
                }
                else
                {
                    if(!isEmpty(&commands[j]))
                    {
                        printf("Executing: %s\n", &commands[j]);
                        j++;
                    }
                    aux = strtok(NULL, ";");
                }
            }
        }
    }

    return 0;
}