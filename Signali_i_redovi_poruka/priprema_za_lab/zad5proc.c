#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <time.h>

#define KEY 10101

struct poruka
{
    long tip;
    int broj;
};

void main()
{
    int redid, n;
    struct poruka bafer;
    bafer.tip = 1;
    srand(time(NULL));

    redid = msgget(KEY, 0666 | IPC_CREAT);
    if(redid < 0)
    {
        printf("Doslo je do gerske pri kreiranju reda poruka");
        exit(1);
    }

    n = (rand() % 10) + 1;
    int i;

    for(i = 0; i < n + 1; i++)
    {
        if(i == n)
            bafer.broj = -1;
        else
            bafer.broj = rand() % 101;

        if(msgsnd(redid, &bafer, sizeof(bafer.broj), 0) < 0)
        {
            printf("doslo je do greske prilikom slanja poruke");
            exit(1);
        }
    }
}