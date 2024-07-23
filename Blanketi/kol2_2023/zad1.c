/*Korišćenjem programskog jezika C napisati Linux program koji kreira dodatni proces dete. Roditelj
generiše slučajne brojeve u opsegu od 0 do 999 i korišćenjem reda poruka šalje procesu detetu.
Brojevi se šalju sa pridruženim prioritetom pri čemu jednocifreni brojevi imaju prioritet 1, dvocifreni
prioritet 2, a trocifreni prioritet 3. Proces dete sumira primljene brojeve u tri različite sume (za svaki
prioritet zasebna suma) tako što sumiranje imaju brojevi prioriteta 1, zatim brojevi prioriteta 2 a tek
na kraju brojevi prioriteta 3. Proces dete sumiranje prekida kada zbir sve tri sume pređe vrednost
50000. O prekidu sumiranja proces dete korišćenjem istog reda poruka obaveštava proces roditelj
kako bi prekinuo generisanje brojeva.*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <time.h>

#define KEY 10101

struct poruka{
    long tip;
    int broj;
};

void main()
{
    srand(time(NULL));
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
        struct poruka buffer2;

        buffer2.broj = 0;
        while(buffer2.broj < 50000)
        {
            buffer.broj = rand() % 1000;
            if(buffer.broj >= 0 &&  buffer.broj <= 9)
                buffer.tip = 1;
            else if(buffer.broj >= 10 && buffer.broj <= 99)
                buffer.tip = 2;
            else if(buffer.broj >= 100 && buffer.broj <= 999)
                buffer.tip = 3;

            printf("Broj poslat: %d\n", buffer.broj);
            if(msgsnd(redid, &buffer, sizeof(int), 0) < 0)
            {
                printf("Greska prilikom slanja poruke iz parent procesa");
                exit(-1);
            }

            msgrcv(redid, &buffer2, sizeof(int), 4, 0);
            printf("Ukupna suma: %d\n", buffer2.broj);
            sleep(1);
        }

        wait(NULL);
        msgctl(redid, IPC_RMID, NULL);
        exit(0);
    }
    else
    {
        struct poruka buffer;
        struct poruka buffer2;
        int sum1 = 0, sum2 = 0, sum3 = 0;

        buffer2.broj = 0;
        while(buffer2.broj < 50000)
        {
            msgrcv(redid, &buffer, sizeof(int), 0, 0);

            if(buffer.tip == 1)
                sum1 += buffer.broj;
            else if(buffer.tip == 2)
                sum2 += buffer.broj;
            else if(buffer.tip == 3)
                sum3 += buffer.broj;

            fflush(stdout);
            printf("sum1: %d, sum2: %d, sum3: %d\n", sum1, sum2, sum3);

            buffer2.tip = 4;
            buffer2.broj = sum1 + sum2 + sum3;
            msgsnd(redid, &buffer2, sizeof(int), 0);
        }
    }
}