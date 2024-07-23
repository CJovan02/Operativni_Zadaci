#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>

#define MUTEX_KEY 10101
#define PROC_A_KEY 10102
#define PROC_B_KEY 10103
#define DUZINA 80

union semun{
    int val;
    struct semid_ds *buf;
    ushort *array;
};
int main()
{
    int mutexid, procaid, procbid, retCode, i;
    union semun semopts;
    struct sembuf sem_lock = {0, -1, 0};
    struct sembuf sem_unlock = {0, 1, 0};
    /* Kreiranje semafora. */
    mutexid = semget((key_t)MUTEX_KEY, 1, 0666 | IPC_CREAT);
    procaid = semget((key_t)PROC_A_KEY, 1, 0666 | IPC_CREAT);
    procbid = semget((key_t)PROC_B_KEY, 1, 0666 | IPC_CREAT);
    /* Inicijalizacija semafora. */
    semopts.val = 1;
    semctl(mutexid, 0, SETVAL, semopts);
    semopts.val = 1;
    semctl(procaid, 0, SETVAL, semopts);
    semopts.val = 0;
    semctl(procbid, 0, SETVAL, semopts);
    if (fork() == 0)
    {
        mutexid = semget((key_t)MUTEX_KEY, 1, 0666);
        procaid = semget((key_t)PROC_A_KEY, 1, 0666);
        procbid = semget((key_t)PROC_B_KEY, 1, 0666);
        for(i = 1; i <= 19; i = i + 2)
        {
            semop(procaid, &sem_lock, 1);
            semop(mutexid, &sem_lock, 1);

            printf("p11 -> %d\n", i);

            semop(mutexid, &sem_unlock, 1);
            semop(procbid, &sem_unlock, 1);
        }
        exit(0);
    }
    else
    {
        mutexid = semget((key_t)MUTEX_KEY, 1, 0666);
        procaid = semget((key_t)PROC_A_KEY, 1, 0666);
        procbid = semget((key_t)PROC_B_KEY, 1, 0666);
        for(i = 0; i <= 20; i = i + 2)
        {
            semop(procbid, &sem_lock, 1);
            semop(mutexid, &sem_lock, 1);

            printf("p22 -> %d\n", i);

            semop(mutexid, &sem_unlock, 1);
            semop(procaid, &sem_unlock, 1);
        }
        wait(&retCode);
        semctl(mutexid, 0, IPC_RMID, 0);
        semctl(procaid, 0, IPC_RMID, 0);
        semctl(procbid, 0, IPC_RMID, 0);
    }
}
/*jedan proces generise parne a drugi neparne brojeve uzastopno, od 0 do 20

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>

#define MUTEX_KEY 10101
#define PARAN_KEY 10102
#define NEPARAN_KEY 10103

union semun{
    int val;
    struct semid_ds *buf;
    ushort *array;
};

int main()
{
    int paranId, neparanId, i, retCode;
    union semun semopts;
    struct sembuf sem_lock = {0, -1, 0};
    struct sembuf sem_unlock = {0, 1, 0};

    //kreiranje semafora
    paranId = semget((key_t)PARAN_KEY, 1, 0666 | IPC_CREAT);
    neparanId = semget((key_t)NEPARAN_KEY, 1, 0666 | IPC_CREAT);
    //inicijalizacija semafora
    semopts.val = 1;
    semctl(paranId, 0, SETVAL, semopts);
    semopts.val = 0;
    semctl(neparanId, 0, SETVAL, semopts);

    if(fork() == 0)
    {
        paranId = semget((key_t)PARAN_KEY, 1, 0666);
        neparanId = semget((key_t)NEPARAN_KEY, 1, 0666);
        for(i = 1; i <= 19; i = i + 2)
        {
            semop(paranId, &sem_lock, 1);

            printf("p1 -> %d\n", i);

            semop(neparanId, &sem_unlock, 1);
        }
        exit(0);
    }
    else
    {
        paranId = semget((key_t)PARAN_KEY, 1, 0666);
        neparanId = semget((key_t)NEPARAN_KEY, 1, 0666);
        for(i = 0; i <= 20; i = i + 2)
        {
            semop(neparanId, &sem_lock, 1);

            printf("p2 -> %d\n", i);

            semop(paranId, &sem_unlock, 1);
        }

        wait(retCode);
        semctl(paranId, 0, IPC_RMID, 0);
        semctl(neparanId, 0, IPC_RMID, 0);
    }

    return 0;
}*/

















