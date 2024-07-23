/*Korišćenjem programskog jezika C napisati UNIX/Linux program koji simulira problem
proizvođač/potrošač korišćenjem redova poruka (message-queues). Glavni program se deli u dva
procesa. Prvi proces (proizvođač) kreira N slučajnih pozitivnih celih brojeva i šalje ih drugom
procesu. N se određuje tokom izvršenja, takođe kao slučajan pozitivan ceo broj. Po završetku
slanja, prvi proces šalje -1 kao kod za kraj. Drugi proces (potrošač) preuzima poslate brojeve iz
poruka i štampa ih na standardnom izlazu.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define KEY 10101
#define LENGTH 10

struct poruka
{
    long tip;
    int broj;
};

void main()
{

    int pid;
    struct poruka bafer;
    int redid;
    int n;

    bafer.tip = 1;

    redid = msgget(KEY, 0666 | IPC_CREAT);
    if(redid < 0)
    {
        printf("Doslo je do gerske pri prekiranju reda poruka");
        exit(1);
    }

    pid = fork();

    if(pid != 0)
    {
        if(execl("zad5proc", "./zad5proc", NULL) < 0)
        {
            printf("Doslo je do greske prilikom otvaranja datoteke");
            exit(1);
        }

        wait(NULL);
        msgctl(redid, IPC_RMID, NULL);
    }
    else
    {
        int i;
        do
        {
            if(msgrcv(redid, &bafer, sizeof(bafer.broj), 0, 0) < 0)
            {
                printf("Doslo je do greske pri primanju poruke");
                exit(1);
            }

            printf("Child process je procitao broj: %d\n", bafer.broj);

        } while (bafer.broj != -1);
        
    }
}