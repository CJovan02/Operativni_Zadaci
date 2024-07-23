/*
Korišćenjem programskog jezika C kreirati tri Linux procesa koji za za međusobnu komunikaciju
koriste deljenu memoriju veličine 1024 bajtova. Prvi proces popunjava prvih 512 bajtova deljive
memorije slučajno izabranim slovima (u opsegu a-z). Nakon toga, drugi proces popunjava
poslednjih 512 bajtova deljene memorije, proizvoljno izabranim ciframa. Pošto i drugi proces
završi generisanje podataka, treći proces kompletan sadržaj deljene memorije upisuje u datoteku.
Ova sekvenca akcija se periodično ponavlja svakih 15 s. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

#define MEM_KEY 1122
#define PROC_A_KEY 10101
#define PROC_B_KEY 10102
#define PROC_C_KEY 10103
#define LENGTH 1024

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void main()
{
    srand(time(NULL));
    int i, procaid, procbid, proccid;
    union semun semopts;
    struct sembuf sem_lock = {0, -1, 0};
    struct sembuf sem_unlock = {0, 1, 0};

    procaid = semget((key_t)PROC_A_KEY, 1, 0666 | IPC_CREAT);
    procbid = semget((key_t)PROC_B_KEY, 1, 0666 | IPC_CREAT);
    proccid = semget((key_t)PROC_C_KEY, 1, 0666 | IPC_CREAT);

    semopts.val = 1;
    semctl(procaid, 0, SETVAL, semopts);
    semopts.val = 0;
    semctl(procbid, 0, SETVAL, semopts);
    semopts.val = 0;
    semctl(proccid, 0, SETVAL, semopts);

    if (fork() != 0) //popunjava prvu polovinu od a-z
    {
        int i;
        int shmid = shmget(MEM_KEY, LENGTH*sizeof(char), IPC_CREAT | 0666);
        char* shmptr = shmat(shmid, NULL, 0);

        while(1)
        {
            semop(procaid, &sem_lock, 1);

            for(i = 0; i < LENGTH / 2; i++)
                shmptr[i] = 'a' + rand() % ('z' - 'a' + 1);

            semop(procbid, &sem_unlock, 1);
        }

        shmdt(shmptr);
        wait(NULL);
        shmctl(shmid, IPC_RMID, NULL);
        semctl(procaid, 0, IPC_RMID, NULL);
        semctl(procbid, 0, IPC_RMID, NULL);
        semctl(proccid, 0, IPC_RMID, NULL);
    } 
    else if(fork() != 0) //popunjava drugu polovinu random brojevima
    {
        int i;
        int shmid = shmget(MEM_KEY, LENGTH*sizeof(char), IPC_CREAT | 0666);
        char* shmptr = shmat(shmid, NULL, 0);
        
        while(1)
        {
            semop(procbid, &sem_lock, 1);
            for(i = LENGTH / 2; i < LENGTH; i++)
                shmptr[i] = '0' + rand() % ('9' - '1' + 1);

            semop(proccid, &sem_unlock, 1);
        }

        shmdt(shmptr);
        wait(NULL);
    }
    else
    {
        int shmid = shmget(MEM_KEY, LENGTH*sizeof(char), IPC_CREAT | 0666);
        char* shmptr = shmat(shmid, NULL, SHM_RDONLY);
        
        char data[LENGTH];

        while(1)
        {
            semop(proccid, &sem_lock, 1);

            memcpy(data, shmptr, LENGTH);
            puts(data);
            sleep(15);

            semop(procaid, &sem_unlock, 1);
        }


        shmdt(shmptr);
    }
    
}