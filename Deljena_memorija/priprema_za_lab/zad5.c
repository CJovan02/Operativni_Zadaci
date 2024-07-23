/*
Korišćenjem programskog jezika C kreirati program koji se deli u dva procesa. Proces roditelj čita
sa tastature stringove i koristeći deljenu memoriju prosleđuje ih procesu detetu. Proces dete
primljene stringove upisuje u datoteku stringovi.txt. Kada korisnik unese string “KRAJ”
komunikacija se prekida i procesi se završavaju.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>

#define MEM_KEY 1122
#define PROC_A_KEY 10101
#define PROC_B_KEY 10102

union semun{
    int val;
    struct semid_ds *buf;
    /*u*/short *array;
};

void main()
{
    int i, procaid, procbid;
    union semun semopts;
    struct sembuf sem_lock = {0, -1, 0};
    struct sembuf sem_unlock = {0, 1, 0};

    procaid = semget((key_t)PROC_A_KEY, 1, 0666 | IPC_CREAT);
    procbid = semget((key_t)PROC_B_KEY, 1, 0666 | IPC_CREAT);

    semopts.val = 1;
    semctl(procaid, 0, SETVAL, semopts);
    semopts.val = 0;
    semctl(procbid, 0, SETVAL, semopts);

    if(fork() != 0)
    {
        char line[80];
        int shmid;
        char* shmptr;

        shmid = shmget(MEM_KEY, 80*sizeof(char), 0666 | IPC_CREAT);
        shmptr = shmat(shmid, NULL, 0);

        while(strcmp(line, "KRAJ") != 0)
        {
            semop(procaid, &sem_lock, 1);

            printf("Unesite string: ");
            gets(line);
            memcpy(shmptr, line, 80);

            semop(procbid, &sem_unlock, 1);
        }
        shmdt(shmptr);

        int status;
        wait(&status);

        shmctl(shmid, IPC_RMID, NULL);
        semctl(procaid, 0, IPC_RMID, NULL);
        semctl(procbid, 0, IPC_RMID, NULL);
    }
    else
    {
        char line[80];
        int shmid = shmget(MEM_KEY, 80*sizeof(char), 0666 | IPC_CREAT);
        char* shmptr = shmat(shmid, NULL, 0);

        while(strcmp(line, "KRAJ") != 0)
        {
            semop(procbid, &sem_lock, 1);

            memcpy(line, shmptr, 80);
            printf("Iz deljene memorije procitano: %s\n", line);
            fflush(stdout);

            semop(procaid, &sem_unlock, 1);
        }

        shmdt(shmptr);
    }
}