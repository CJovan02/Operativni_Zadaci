/*
Korišćenjem programskog jezika C kreirati dva Linux procesa. Prvi proces kreira u deljenom
memorijskom segmentu matricu dimenzija MxN. Proces 2 u svakoj koloni matrice pronalazi
maksimalni element i štampa ga na ekranu.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>

#define N 4
#define M 5
#define MEM_KEY 1122
#define SEM_KEY 3344

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void main()
{
    srand(time(NULL));
    int semid;
    union semun semopts;

    struct sembuf semLock = {0, -1, 0};
    struct sembuf semUnlock = {0, 1, 0};
    
    semid = semget((key_t)SEM_KEY, 1, 0666 | IPC_CREAT);
    semopts.val = 0;
    semctl(semid, 0, SETVAL, semopts);

    if(fork() != 0)
    {
        int i, j;

        int shmid = shmget(MEM_KEY, sizeof(int)*M*N, 0666 | IPC_CREAT);
        int* shmptr = shmat(shmid, NULL, 0);

        for(i = 0; i < N; i++)
        {
            for(j = 0; j < M; j++)
            {
                shmptr[i*M+j] = rand() % 100;
                printf("%d\t", shmptr[i*M+j]);
            }
            printf("\n");
        }
        semop(semid, &semUnlock, 1);

        shmdt(shmptr);
        wait(NULL);
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID, NULL);
    }
    else
    {
        int i, j;
        int shmid = shmget(MEM_KEY, M*N*sizeof(int), 0666 | IPC_CREAT);
        int* shmptr = shmat(shmid, NULL, 0);

        semop(semid, &semLock, 1);
        for(i = 0; i < N; i++)
        {
            int max = 0;
            for(j = 0; j < M; j++)
            {
                if(shmptr[i*M+j] > max)
                    max = shmptr[i*M+j];
            }
            printf("Maksimalni element kolone %d je %d\n", i, max);
        }

        shmdt(shmptr);
    }
}