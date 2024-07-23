#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
    char* args[4];
    char file[100];

    printf("Unesite ime datoteke: ");
    scanf("%s", file);

    if(fork() == 0)
    {
        args[0] = (char*)malloc(100 * sizeof(char));
        args[0] = "cp";

        args[1] = (char*)malloc(100 * sizeof(char));
        strcpy(args[1], file);

        args[2] = (char*)malloc(100 * sizeof(char));
        args[2] = "./tmp";

        args[3] = NULL;

        if(execvp("cp", args) < 0)
        {
            printf("Doslo je do greske!");
            exit(1);
        }

    }
    else
        wait(NULL);

    return 0;
}
