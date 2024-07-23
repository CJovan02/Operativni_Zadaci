/*
Korišćenjem programskog jezika C napisati Linux/UNIX program koji kreira jedan dodatni proces. Ova dva procesa 
komuniciraju korišćenjem deljive memorije veličine dva int broja. Proces roditelj sa tastature čita jedan broj, 
a u deljenu memoriju upisuje taj broj i njegovu trostruku vrednost. Zatim proces dete čita ove brojeve i na 
ekranu štampa njihov zbir. Ovaj ciklus se ponavlja sve dok se sa tastature ne unese ključna reč KRAJ.*/

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
#define MEM_KEY 10103
#define LENGTH 80

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void main()
{
    int procaid, procbid;
    
    union semun semopts;
    struct sembuf semLock = {0, -1, 0};
    struct sembuf semUnlock = {0, 1, 0};

    procaid = semget((key_t)PROC_A_KEY, 1, 0666 | IPC_CREAT);
    procbid = semget((key_t)PROC_B_KEY, 1, 0666 | IPC_CREAT);

    semopts.val = 1;
    semctl(procaid, 0, SETVAL, semopts);
    semopts.val = 0;
    semctl(procbid, 0, SETVAL, semopts);

    if(fork() != 0)
    {
        int shmid = shmget(MEM_KEY, sizeof(int) * 2, 0666 | IPC_CREAT);
        int* shmptr = shmat(shmid, NULL, 0);

        char line[LENGTH];
        int poruka[2];

        while(1)
        {
            semop(procaid, &semLock, 1);

            gets(line);

            if(strcmp(line, "KRAJ") == 0)
            {
                semop(procaid, &semUnlock, 1);
                break;
            }

            poruka[0] = atoi(line);
            poruka[1] = poruka[0] * 3;
            
            memcpy(shmptr, poruka, sizeof(int) * 2);

            semop(procbid, &semUnlock, 1);
        }
        semop(procaid, &semLock, 1);

        poruka[0] = -1;
        memcpy(shmptr, poruka, sizeof(int) * 2);

        semop(procbid, &semUnlock, 1);

        shmdt(shmptr);
        wait(NULL);
        shmctl(shmid, IPC_RMID, NULL);
        semctl(procaid, 0, IPC_RMID, NULL);
        semctl(procbid, 0, IPC_RMID, NULL);
        exit(0);
    }
    else
    {
        int shmid = shmget(MEM_KEY, sizeof(int) * 2, 0666 | IPC_CREAT);
        int* shmptr = shmat(shmid, NULL, 0);

        int poruka[2];
        poruka[0] = 0;

        while(poruka[0] != -1)
        {
            semop(procbid, &semLock, 1);

            memcpy(poruka, shmptr, sizeof(int) * 2);
            if(poruka[0] != -1)
                printf("%d + %d = %d\n", poruka[0], poruka[1], poruka[0] + poruka[1]);

            semop(procaid, &semUnlock, 1);
        }

        shmdt(shmptr);
    }
}