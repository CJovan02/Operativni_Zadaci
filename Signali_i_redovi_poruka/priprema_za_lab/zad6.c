/*Korišćenjem programskog jezika C napisati UNIX/Linux program koji učitava podatke iz tekstualne
datoteke cela (red po red) i zatim korišćenjem reda poruka sve parne redove šalje procesu koji
konvertuje sva slova u velika i zapisuje ih u datoteku pola1, a sve neparne redove procesu koji
konvertuje sva slova u mala i zapisuje ih u datoteku pola2.*/

//nekad radi nekad ne, nemam pojma :D
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>

#define KEY 10101
#define MAX_PORUKA 255

struct poruka
{
    long tip;
    char tekst[MAX_PORUKA];
};


void main()
{
    int pid1, pid2, redid;
    FILE* ulaz, *pola1, *pola2;
    int count = 1;

    redid = msgget(KEY, 0666 | IPC_CREAT);
    if(redid < 0)
    {
        printf("Greska pri kreiranju reda poruka");
        exit(1);
    }

    if(pid1 = fork() != 0)
    {
        ulaz = fopen("zad6ulaz.txt", "r");
        struct poruka bafer;

        if(ulaz == NULL)
        {
            printf("Greska pri otvaranju fajla: zad6ulaz.txt");
            exit(1);
        }

        while(!feof(ulaz))
        {
            fgets(bafer.tekst, MAX_PORUKA, ulaz);
            if(count % 2 == 0)
                bafer.tip = 1;
            else
                bafer.tip = 2;

            if(msgsnd(redid, &bafer, strlen(bafer.tekst) + 1, 0) < 0)
            {
                printf("Greska pri slanju poruke");
                exit(1);
            }
            count++;
        }
        //upisujemo kod za kraj za oba procesa
        strcpy(bafer.tekst, "ENDOFFILE");

        bafer.tip = 1;
        msgsnd(redid, &bafer, 10, 0);
        bafer.tip = 2;
        msgsnd(redid, &bafer, 10, 0);

        fclose(ulaz);
        wait(NULL);
        msgctl(redid, IPC_RMID, NULL);
        exit(0);
    }
    else if(pid2 = fork() != 0)
    {
        if(execl("zad6proc", "./zad6proc", "1", "pola1.txt", NULL) < 0)
        {
            printf("Greska pri otvaranju programa");
            exit(1);
        }
    }
    else
    {
        if(execl("zad6proc", "./zad6proc", "2", "pola2.txt", NULL) < 0)
        {
            printf("Greska pri otvaranju programa");
            exit(1);
        }
    }
}