/*
Korišćenjem programskog jezika C kreirati tri Linux procesa koji komuniciraju korišćenjem redova poruka. 
Prvi proces prvo cifru pa string sa tastature. Ukoliko je pročitana cifra jednaka 1, korišćenjem redova poruka 
pročitani string šalje drugom procesu a ukoliko je uneta cifra jednaka 2, poruku šaljem trećem procesu. 
Drugi proces ispisuje svaku primljenu poruku i njen broj karaktera. Treći proces primljenu poruku pretvara u 
velika slova a zatim je ispisuje. Komunikacija se prekida i procesi izlaze kada korisnik unese cifru različitu 
od 1 ili 2.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <string.h>
#include <ctype.h>

#define KEY 10101
#define LENGTH 255

struct poruka{
    long tip;
    char tekst[LENGTH];
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

        int broj = 0;
        char linija[80];

        while(1)
        {
            sleep(1);
            printf("Unesite broj: \n");
            fgets(linija, 80, stdin);
            buffer.tip = atoi(linija);

            if(buffer.tip != 1 && buffer.tip != 2)
                break;

            printf("Unesite poruku koju zelite da prosledite: \n");
            fgets(buffer.tekst, LENGTH, stdin);

            if(msgsnd(redid, &buffer, LENGTH, 0) < 0)
            {
                printf("Greska pri slanju poruke");
                exit(-1);
            }
        }

        strcpy(buffer.tekst, "END");
        buffer.tip = 1;
        msgsnd(redid, &buffer, 4, 0);
        buffer.tip = 2;
        msgsnd(redid, &buffer, 4, 0);

        wait(NULL);
        msgctl(redid, IPC_RMID, NULL);
        exit(0);
    }
    else if(fork() != 0) //ispisuje poruku i broj karaktera te poruke
    {
        struct poruka buffer;

        while(strcmp(buffer.tekst, "END") != 0)
        {
            msgrcv(redid, &buffer, LENGTH, 1, 0);

            if(strcmp(buffer.tekst, "END") != 0)
            {
                printf("Prvi proces primio poruku: \n%s", buffer.tekst);
                printf("Broj karaktera: %ld\n\n", strlen(buffer.tekst) - 1);
            }
        }

        wait(NULL);
    }
    else //pretvara poruku u velika slova i ispisuje
    {
        struct poruka buffer;

        while(strcmp(buffer.tekst, "END") != 0)
        {
            msgrcv(redid, &buffer, LENGTH, 2, 0);

            if(strcmp(buffer.tekst, "END") != 0)
            {
                int i;
                for(i = 0; i < strlen(buffer.tekst); i++)
                    buffer.tekst[i] = toupper(buffer.tekst[i]);

                printf("Drugi proces primio poruku: \n%s\n", buffer.tekst);
            }
        }
    }
}