#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUMARGS 20

int main()
{
    char command[100];
    char *args[NUMARGS];
    int noArgs = 0;
    int retStatus;

    printf("/nprompt> ");
    gets(command);

    args[noArgs++] = strtok(command, " ");
    while((args[noArgs++] = strtok(0, " ")) != NULL)
    {}

    while(strcmp(command, "logout") != 0)
    {
        if( fork() == 0)
        {
            sleep(5);
            if(strcmp(args[noArgs - 2], "&") == 0)
                args[noArgs - 2] = NULL;

            execvp(args[0], args);
            printf("Greska pri izvrsavanju komande");
            exit(-1);
        }
        else
        {
            if(strcmp(args[noArgs - 2], "&") == 0)
                wait(&retStatus);
        }

        printf("/nprompt> ");
        gets(command);

        noArgs = 0;
        args[noArgs++] = strtok(command, " ");
        while((args[noArgs++] = strtok(0, " ")) != NULL)
        {}
    }
}
