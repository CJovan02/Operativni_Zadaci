/* Koriscenjem programskog jezika C napisati UNIX/Linux program koj svom procesu detetu korisnjecem redova
   poruka identifikovan brojev 12002 prosledjuje celobrojne vrednosti vece od nule koje cita sa tasature.
   Poruka su tipa 3. Proces dobijene vrednosti upisuje u datoteku izlaz.txt. Komunikacija se prekida kada 
   korisnik sa tastature unese broj 787 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>

#define KEY 12002

struct poruka
{
    long tip;
    int broj;
};

void main()
{
    int pid, redid;

    redid = msgget(KEY, 0666 | IPC_CREAT);
    if(redid < 0)
    {
        printf("Greska pri kreiranju reda poruka");
        exit(1);
    }

    if(pid = fork() != 0)
    {
        struct poruka buffer;
        buffer.broj = 0;
        buffer.tip = 3;
        char linija[50];

        while(buffer.broj != 787)
        {
            sleep(1);
            do
            {
                printf("Unesite celobrojnu vrednost vecu od nule: ");
                gets(linija);
                buffer.broj = atoi(linija);
            } while(buffer.broj < 0);

            if(msgsnd(redid, &buffer, sizeof(int), 0) < 0)
            {
                printf("Greska pri slanju poruke");
                exit(1);
            }
        }

        wait(NULL);
        msgctl(redid, IPC_RMID, NULL);
        exit(0);
    }
    else
    {
        struct poruka buffer;
        buffer.broj = 0;

        while(buffer.broj != 787)
        {
            if(msgrcv(redid, &buffer, sizeof(int), 3, 0) < 0)
            {
                printf("Greska pri prijemu poruke");
                exit(1);
            }

            if(buffer.broj != 787)
            printf("Child process je primio broj %d\n", buffer.broj);
        }
    }
}