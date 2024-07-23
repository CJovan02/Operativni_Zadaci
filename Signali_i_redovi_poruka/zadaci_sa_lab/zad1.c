/*Korišćenjem programskog jezika C napisati UNIX/Linux program koji simulira problem proizvođač/potrošač
  korišćenjem redova poruka (message-queues). Glavni program se deli u dva procesa. Prvi proces (proizvođač) 
  kreira N slučajnih pozitivnih celih brojeva i šalje ih drugom procesu. N se određuje tokom izvršenja, 
  takođe kao slučajan pozitivan ceo broj. Po završetku slanja, prvi proces šalje -1 kao kod za kraj. 
  Drugi proces (potrošač) preuzima poslate brojeve iz poruka i štampa ih na standardnom izlazu. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <time.h>

#define KEY 10101

struct poruka
{
    long tip;
    int broj;
};

void main()
{
    srand(time(NULL));
    int pid, redid;

    redid = msgget(KEY, 0666 | IPC_CREAT);
    if(redid < 0)
    {
        printf("Greska pri kreiranju reda poruka");
        exit(1);
    }

    if(pid = fork() != 0)
    {
        int n = (rand() % 20) + 1;
        int i;
        struct poruka bafer;
        bafer.tip = 1;

        for(i = 0; i < n; i++)
        {
            bafer.broj = rand() % 101;
            if(msgsnd(redid, &bafer, sizeof(int), 0) < 0)
            {
                printf("Greska pri slanju poruke");
                exit(1);
            }
        }

        bafer.broj = -1;
        if(msgsnd(redid, &bafer, sizeof(int), 0) < 0)
        {
            printf("Greska pri slanju poruke");
            exit(1);
        }

        wait(NULL);
        msgctl(redid, IPC_RMID, NULL);
    }
    else
    {
        struct poruka bafer;

        bafer.broj = 0;
        while(bafer.broj != -1)
        {
            if(msgrcv(redid, &bafer, sizeof(int), 0, 0) < 0)
            {
                printf("Greska pri prijemu poruke");
                exit(1);
            }
            
            if(bafer.broj != -1)
                printf("%d, ", bafer.broj);
        }
        printf("\n");
    }
}