/*
Korišćenjem programskog jezika C kreirati program koji kopira zadatu datoteku. Prvi argument
komandne linije je naziv originalne datoteke, a drugi argument je naziv nove datoteke, koja će biti
kopija prve. Kopiranje se obavlja tako što se program deli u dva procesa, prvi proces čita određeni
broj bajtova iz datoteke i šalje ih drugom procesu. Drugi proces bajtove iz deljene memorije upisuje
u novu datoteku.*/

//resenje ne radi kako su radili u pdf-u, umesto da prenosi odredjeni broj bajtova
//on prenosi liniju po liniju iz prve txt datoteke

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>
#include <string.h>

#define MEM_KEY 10101
#define SEM_A_KEY 10102
#define SEM_B_KEY 10103
#define LENGTH 255

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Pogresno uneseni argumenti programa\n");
        exit(1);
    }

    union semun semopts;
    struct sembuf semLock = {0, -1, 0};
    struct sembuf semUnlock = {0, 1, 0};

    int procaid, procbid;
    procaid = semget((key_t)SEM_A_KEY, 1, 0666 | IPC_CREAT);
    procbid = semget((key_t)SEM_B_KEY, 1, 0666 | IPC_CREAT);

    semopts.val = 1;
    semctl(procaid, 0, SETVAL, semopts);
    semopts.val = 0;
    semctl(procbid, 0, SETVAL, semopts);

    if(fork() != 0)
    {
        int shmid = shmget(MEM_KEY, sizeof(char)*LENGTH, 0666 | IPC_CREAT);
        char* shmptr = shmat(shmid, NULL, 0);

        FILE* ulaz = fopen(argv[1], "r");
        if(ulaz == NULL)
        {
            printf("kurcina");
            exit(1);
        }

        char line[LENGTH];

        while(!feof(ulaz))
        {
            semop(procaid, &semLock, 1);

            fgets(line, LENGTH, ulaz);
            fflush(stdout);
            //printf("%s\n", line);
            memcpy(shmptr, line, LENGTH);

            semop(procbid, &semUnlock, 1);
        }

        semop(procaid, &semLock, 1);

        memcpy(shmptr, "ENDOFFILE", 10);

        semop(procbid, &semUnlock, 1);

        fclose(ulaz);
        shmdt(shmptr);

        int status;
        wait(&status);
        shmctl(shmid, IPC_RMID, NULL);
        semctl(procaid, 0, IPC_RMID, NULL);
        semctl(procbid, 0, IPC_RMID, NULL);
        exit(0);
    }
    else
    {
        int shmid = shmget(MEM_KEY, sizeof(char)*LENGTH, 0666 | IPC_CREAT);
        char* shmptr = shmat(shmid, NULL, 0);

        FILE* izlaz = fopen(argv[2], "w");
        if(izlaz == NULL)
        {
            printf("opet kurcina druze");
            exit(1);
        }
        char line[LENGTH];

        while(strcmp(line, "ENDOFFILE") != 0)
        {
           
            semop(procbid, &semLock, 1);

            memcpy(line, shmptr, LENGTH);
            if(strcmp(line, "ENDOFFILE") != 0)
                fprintf(izlaz, "%s", line);

            semop(procaid, &semUnlock, 1);
        }

        fclose(izlaz);
        shmdt(shmptr);
    }
}