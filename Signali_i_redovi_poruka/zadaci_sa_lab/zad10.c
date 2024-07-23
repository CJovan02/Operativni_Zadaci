/*Korišćenjem programskog jezika C napisati Linux program koji treba da detektuje i obradi signale tipa
  SIGALRM i SIGINT. Kada proces detektuje signal tipa SIGALRM on korišćenjem reda poruka (koji je definisan
  ključem 10500) šalje poruku (tip 20) koja sadrži informaciju o tekućem vremenu. U slučaju pojave SIGINT 
  signala (pritisnuta kombinacija tastera Ctrl+C) proces suspenduje svoje izvršavanje na 5 sekundi a nakon
  toga briše red poruka (koji je koristio za slanje informacija o vremenu) prekida svoje izvršavanje.*/

//nije lepo objasnjeno u zadatku da li se koriste 2 procesa i kako oni komuniciraju jbg
//ovde je uradjeno tako da roditelj kada primi sigalrm signal salje informaciju
//drugom procesu koji stampa tu poruku

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#define KEY 10500
#define LENGTH 255

struct poruka
{
    long tip;
    char tekst[LENGTH];
};

void alarmHandler(int sigNum)
{
    struct poruka bafer;

    time_t vreme;

    time(&vreme);
    strcpy(bafer.tekst, ctime(&vreme));
    bafer.tip = 20;

    int redid = msgget(KEY, 0666 | IPC_CREAT);
    if(msgsnd(redid, &bafer, LENGTH, 0) < 0)
    {
        printf("Greska prilikom slanja poruke");
        exit(1);
    }
}

void intHandler(int sigNum)
{
    printf(" Gasim program, molimo vas da sacekate...\n");

    //mora alarm da se vrati na 0 jer sleep radi tako sto suspenduje proces sve dok ne istekne vreme
    //ili se primi signal koji se ne odbacuje
    alarm(0);
    sleep(5);
    int redid = msgget(KEY, 0666 | IPC_CREAT);
    msgctl(redid, IPC_RMID, NULL);
    exit(0);
}

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
        struct poruka bafer;

        signal(SIGALRM, alarmHandler);
        signal(SIGINT, intHandler);

        while(1)
        {
            alarm(2);
            pause();
        }
        
        wait(NULL);
        msgctl(redid, IPC_RMID, NULL);
        exit(0);
    }
    else 
    {
        struct poruka bafer;

        while(1)
        {
            if(msgrcv(redid, &bafer, LENGTH, 20, 0) < 0)
            {
                printf("Greska prilikom prijema poruke");
                exit(1);
            }
            printf("%s", bafer.tekst);
        }
    }
}