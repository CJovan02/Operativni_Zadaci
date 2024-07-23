/*KoriScenjem programskog jezika C kreirati dva Linux procesa (prodes roditelj i proces dete) koji
komuniciraju koris¢enjem mehanizma redova poruka (message queues). Prvi proces ¢ita brojeve sa tastature i
upisuje ih u red poruka sve dok se na tastaturi ne unese broj 0. Brojevi koji se unose su u intervalu [0-127]. Drugi
proces Cita brojeve izreda poruka i na standardnom izlazu ispisuje ASCII karakter koji odgovara dobijenom broju.
Nakon Sto drugi proces dobije iz reda poruka broj 0, ispisuje koliko je ukupno brojeva primljeno preko reda
poruka i izlazi.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define KEY 10101

struct poruka{
    long tip;
    int broj;
};

void main()
{
    int redid;

    redid = msgget(KEY, IPC_CREAT | 0666);
    if(redid < 0)
    {
        printf("Greska pri kreiranju reda poruka");
        exit(-1);
    }

    if(fork() != 0)
    {
        struct poruka buffer;
        buffer.broj = 1;
        buffer.tip = 1;

        char linija[80];
        while(buffer.broj != 0)
        {
            sleep(1);
            do
            {
                printf("Unesite broj u intervalu od 0 - 127: ");
                fgets(linija, 80, stdin);
                buffer.broj = atoi(linija);
            } while(buffer.broj < 0 || buffer.broj > 127);

            if(msgsnd(redid, &buffer, sizeof(int), 0) < 0)
            {
                printf("Greska pri slanju poruke\n");
                exit(-1);
            }            
        }

        wait(NULL);
        msgctl(redid, IPC_RMID, NULL);
        exit(0);
    }
    else
    {
        struct poruka buffer;
        buffer.broj = 1;
        int count = 0;
        char cBuffer[100];

        while(buffer.broj != 0)
        {
            if(msgrcv(redid, &buffer, sizeof(int), 0, 0) < 0)
            {
                printf("Greska pri prijemu poruke\n");
                exit(-1);
            }
            count++;

            fflush(stdout);
            sprintf(cBuffer, "%c", buffer.broj);
            printf("ASCII za prosledjen broj: %s\n", cBuffer);
        }

        printf("Ukupno primljeno poruka: %d\n", count);
    }
}