#include "mySHfunc.h"

int main(int argc, char *argv[])
{
    if(argc == 2) /*modo batch*/
    {
        FILE *file;

        file = fopen(argv[1], "r");

        if(file == NULL)
        {
            printf("File not found.\n");
            fclose(file);
            exit(0);
        }

        if(isExit(file))
        {
            fclose(file);

            file = fopen(argv[1], "r");

            printf("Exit command found!\n");
            printFile(file);
            exit(1);
        }
        else
        {
            printf("File has no exit command.\n");
            fclose(file);
            exit(0);
        }
    }
    else /*modo interativo*/
    {

    }

    return 0;
}