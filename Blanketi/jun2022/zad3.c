/*
  Napisati program koji kreira dodatni proces dete. ova dva procesa komuniciraju kiriscenjem jednog reda 
  poruka. Prvi proces sa tastature cita cele visecifrene brojeve i salje ih drugom procesu koristeci red 
  poruka. Drugo proces treba da sabere cifre primljenog broja i da ih odsampa na standardnom izlazu. 
  Ovaj postupak se ponavlja 10 puta.*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>

#define KEY 10101

struct poruka {
    long tip;
    int broj;
};

void main()
{
    int redid = msgget(KEY, 0666 | IPC_CREAT);
    if(redid < 0)
    {
        printf("Greska pri kreiranju reda poruka");
        exit(-1);
    }

    if(fork() != 0)
    {
        struct poruka buffer;
        buffer.tip = 1;

        char line[80];
        int i;
        for(i = 0; i < 10; i++)
        {
            sleep(1);
            do
            {
                printf("Unesite visecifreni broj: ");
                fgets(line, 80, stdin);
                buffer.broj = atoi(line);

            } while(buffer.broj < 10);

            if(msgsnd(redid, &buffer, sizeof(int), 0) < 0)
            {
                printf("Greska pri slanju poruka");
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
        
        int i, broj, sum = 0;
        for(i = 0; i < 10; i++)
        {
            if(msgrcv(redid, &buffer, sizeof(int), 0, 0) < 0)
            {
                printf("Greska pri prijemu poruke");
                exit(-1);
            }
            
            broj = buffer.broj;
            sum = 0;
            
            while(broj > 0)
            {
                sum += broj % 10;
                broj /= 10;
            }

            printf("Suma cifara prosledjenog broja: %d\n", sum);
        }
    }
}