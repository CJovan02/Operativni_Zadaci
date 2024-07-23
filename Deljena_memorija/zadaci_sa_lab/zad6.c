/*
Korišćenjem programskog jezika C napisati Unix/Linux program koji kao argument komandne linije uzima naziv
datoteke u tekućem direktorijumu koja sadrži više linija teksta. Program se deli na dva procesa
pri čemu proces roditelj otvara tekstualnu datoteku i čita iz nje liniju po liniju teksta i korišćenjem
deljene memorije pročitanu liniju teksta prosleđuje procesu detetu. Proces dete štampa na standardnom izlazu 
(monitoru) linije teksta koje prihvata iz deljene memorije. Kako bi obezbedili da proces dete štampa linije 
teksta onim redosledom kako se javljaju u tekstualnoj datoteci sinhronizovati procese korišćenjem
IPC System V semafora.*/

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
#define LENGTH 255

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
        int shmid = shmget(MEM_KEY, sizeof(char) * LENGTH, 0666 | IPC_CREAT);
        char* shmptr = shmat(shmid, NULL, 0);

        FILE* file = fopen("zad6ulaz.txt", "r");

        char line[LENGTH];
        while(!feof(file))
        {
            semop(procaid, &semLock, 1);

            fgets(line, LENGTH, file);
            memcpy(shmptr, line, LENGTH);

            semop(procbid, &semUnlock, 1);
        }
        semop(procaid, &semLock, 1);

        memcpy(shmptr, "ENDOFFILE", 10);

        semop(procbid, &semUnlock, 1);

        shmdt(shmptr);
        fclose(file);
        wait(NULL);
        shmctl(shmid, IPC_RMID, NULL);
        semctl(procaid, 0, IPC_RMID, NULL);
        semctl(procbid, 0, IPC_RMID, NULL);
        exit(0);
    }
    else
    {
        int shmid = shmget(MEM_KEY, sizeof(char) * LENGTH, 0666 | IPC_CREAT);
        char* shmptr = shmat(shmid, NULL, 0);

        char line[LENGTH];
        while(strcmp(line, "ENDOFFILE") != 0)
        {
            semop(procbid, &semLock, 1);

            memcpy(line, shmptr, LENGTH);
            if(strcmp(line, "ENDOFFILE") != 0)
                printf("%s", line);

            semop(procaid, &semUnlock, 1);
        }

        shmdt(shmptr);
    }
}

// void main()
// {
//     int procaid, procbid;
    
//     union semun semopts;
//     struct sembuf semLock = {0, -1, 0};
//     struct sembuf semUnlock = {0, 1, 0};

//     procaid = semget((key_t)PROC_A_KEY, 1, 0666 | IPC_CREAT);
//     procbid = semget((key_t)PROC_B_KEY, 1, 0666 | IPC_CREAT);

//     semopts.val = 1;
//     semctl(procaid, 0, SETVAL, semopts);
//     semopts.val = 0;
//     semctl(procbid, 0, SETVAL, semopts);

//     if(fork() != 0)
//     {
        // int shmid = shmget(MEM_KEY, sizeof(char) * LENGTH, 0666 | IPC_CREAT);
        // char* shmptr = shmat(shmid, NULL, 0);

        // FILE* file = fopen("zad6ulaz.txt", "r");
        // char line[LENGTH];

        // while(!feof(file))
        // {
        //     semop(procaid, &semLock, 1);

        //     fgets(line, LENGTH, file);
        //     memcpy(shmptr, line, LENGTH);

        //     semop(procbid, &semUnlock, 1);
        // }
        // semop(procaid, &semLock, 1);

        // memcpy(shmptr, "ENDOFFILE", 10);

        // semop(procbid, &semUnlock, 1);

        // shmdt(shmptr);
        // fclose(file);
        // wait(NULL);
        // shmctl(shmid, IPC_RMID, NULL);
        // semctl(procaid, 0, IPC_RMID, NULL);
        // semctl(procbid, 0, IPC_RMID, NULL);
        // exit(0);
//     }
//     else
//     {
//         int shmid = shmget(MEM_KEY, sizeof(char) * LENGTH, 0666 | IPC_CREAT);
//         char* shmptr = shmat(shmid, NULL, 0);

//         char line[LENGTH];

//         while(strcmp(line, "ENDOFFILE") != 0)
//         {
//             semop(procbid, &semLock, 1);

//             memcpy(line, shmptr, LENGTH);
//             if(strcmp(line, "ENDOFFILE") != 0)
//                 printf("%s", line);

//             semop(procaid, &semUnlock, 1);
//         }

//         shmdt(shmptr);
//     }
// }