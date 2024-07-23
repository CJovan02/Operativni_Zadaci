/*
Na programskom jeziku C napisati program koji kreira dva dodatna procesa. Prvi proces dete čita liniju po
liniju iz tekstualne datoteke ulaz1.txt. Drugi proces dete čita liniju po liniju teksta iz tekstualne datoteke 
ulaz2.txt. Oba procesa deteta pročitane linije teksta prosleđuju roditeljskom procesu koristeći deljivi
memorijski prostor. Roditeljski proces primljene linije teksta od oba procesa deteta upisuje u tekstualnu
datoteku izlaz.txt. Sinhronizovati rad ova tri procesa tako da izlazna datoteka izlaz.txt sadrži naizmenično
linije teksta iz adatoteke ulaz1.txt i ulaz2.txt.*/

/*1. nacin: prvo cita iz ulaz1 pa prosledjuje roditelju koji upisuje u fajl
  onda cita iz ulaz2 salje roditelju a on upisuje u fajl itd...*/

/*2. nacin: prvo cita iz ulaz1 i upisuje u mem, cita iz ulaz2 i upisuje u mem
  onda roditelj upisuje obe linje itd...*/

//1. nacin:

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>

#define PROC_A_KEY 10101
#define PROC_B_KEY 10102
#define PROC_C_KEY 10104
#define MEM_KEY 10103
#define LENGTH 255

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void main()
{
    int procaid, procbid, proccid;
    
    union semun semopts;
    struct sembuf semLock = {0, -1, 0};
    struct sembuf semUnlock = {0, 1, 0};

    procaid = semget((key_t)PROC_A_KEY, 1, 0666 | IPC_CREAT);
    procbid = semget((key_t)PROC_B_KEY, 1, 0666 | IPC_CREAT);
    proccid = semget((key_t)PROC_C_KEY, 1, 0666 | IPC_CREAT);

    semopts.val = 0;
    semctl(procaid, 0, SETVAL, semopts);
    semopts.val = 1;
    semctl(procbid, 0, SETVAL, semopts);
    semopts.val = 0;
    semctl(proccid, 0, SETVAL, semopts);

    if(fork() != 0)
    {
        //proces koji upisuje u izlazni fajl
        int shmid = shmget(MEM_KEY, sizeof(char) * LENGTH, 0666 | IPC_CREAT);
        char* shmptr = shmat(shmid, NULL, SHM_RDONLY);

        char line[LENGTH];
        FILE* file = fopen("zad5izlaz.txt", "w");
        int count = 0;

        while(strcmp(line, "ENDOFFILE") != 0)
        {
            semop(procaid, &semLock, 1);

            memcpy(line, shmptr, LENGTH);
            //line[LENGTH] = '\0';
            if(strcmp(line, "ENDOFFILE") != 0)
                fputs(line, file);

            if(count % 2 == 0)
                semop(proccid, &semUnlock, 1);
            else
                semop(procbid, &semUnlock, 1);
            count++;
        }

        fclose(file);
        shmdt(shmptr);
        wait(NULL);
        shmctl(shmid, IPC_RMID, NULL);
        semctl(procaid, 0, IPC_RMID, NULL);
        semctl(procbid, 0, IPC_RMID, NULL);
        semctl(proccid, 0, IPC_RMID, NULL);
        exit(0);
    }
    else if(fork() != 0)
    {
        //proces koji cita iz ulaz1
        int i;
        int shmid = shmget(MEM_KEY, sizeof(char) * LENGTH, 0666 | IPC_CREAT);
        char* shmptr = shmat(shmid, NULL, 0);

        char line[LENGTH];
        FILE* file = fopen("ulaz1.txt", "r");

        while(!feof(file))
        {
            semop(procbid, &semLock, 1);
            
            fgets(line, LENGTH, file);
            memcpy(shmptr, line, LENGTH);
            // for(i = 0; i < LENGTH; i++)
            //     shmptr[i] = line[i];

            semop(procaid, &semUnlock, 1);
        }
        //ulaz1 i ulaz2 moraju da imaju isti broj linija
        //u tom slucaju samo prvi proces salje kod za kraj
        semop(procbid, &semLock, 1);
            
        memcpy(shmptr, "ENDOFFILE", 10);

        semop(procaid, &semUnlock, 1);

        fclose(file);
        shmdt(shmptr);
        wait(NULL);
    }
    else
    {
        //proces koji cita iz ulaz2
        int i;
        int shmid = shmget(MEM_KEY, sizeof(char) * LENGTH, 0666 | IPC_CREAT);
        char* shmptr = shmat(shmid, NULL, 0);

        char line[LENGTH];
        FILE* file = fopen("ulaz2.txt", "r");

        while(!feof(file))
        {
            semop(proccid, &semLock, 1);

            fgets(line, LENGTH, file);
            memcpy(shmptr, line, LENGTH);
            // for(i = 0; i < LENGTH; i++)
            //     shmptr[i] = line[i];

            semop(procaid, &semUnlock, 1);
        }

        fclose(file);
        shmdt(shmptr);
    }
}