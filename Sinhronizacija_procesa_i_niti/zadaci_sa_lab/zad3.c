/*
 Kreirati dva Linux procesa koji generisu slucajne celobrojne vrednosti i upisuju ih u
 datoteku brojevi.dat. Prvi proces generise niz od tri pozitivne celobrojne vrednosti
 i upisuje ih u datoteku. Nakon toga drugi proces gnerise jednu negativnu celobrojnu
 vrednost i upisuje je u datoteku. Postupak se ponavalja 20 puta (naizmenicno prvi pa drugi proces).
 */

//NE RADI MAMU MU JEBEM U PICKU 2 SATA TRAZIM GRESKU
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>

#define POZIVAN_KEY 10101
#define NEGATIVAN_KEY 10102
#define MUTEX_KEY 10103

union semun{
    int val;
    struct semid_ds *buf;
    ushort *array;
};

void main()
{
    srand(time(NULL));
    int pozitivanId, negativanId, mutexId, niz[3], data, i, retCode, k;
    union semun semopts;
    FILE *file;
    struct sembuf sem_lock = {0, -1, 0};
    struct sembuf sem_unlock = {0, 1, 0};

    //kreiranje semafora
    pozitivanId = semget((key_t)POZIVAN_KEY, 1, 066 | IPC_CREAT);
    negativanId = semget((key_t)NEGATIVAN_KEY, 1, 066 | IPC_CREAT);
    mutexId = semget((key_t)MUTEX_KEY, 1, 066 | IPC_CREAT);
    //inicijalizacija semafora
    semopts.val = 1;
    semctl(pozitivanId, 0, SETVAL, semopts);
    semopts.val = 0;
    semctl(negativanId, 0, SETVAL, semopts);
    semopts.val = 0;
    semctl(mutexId, 0, SETVAL, semopts);

    if(fork() == 0)
    {
        pozitivanId = semget((key_t)POZIVAN_KEY, 1, 066);
        negativanId = semget((key_t)NEGATIVAN_KEY, 1, 066);
        mutexId = semget((key_t)MUTEX_KEY, 1, 066);

        for(k = 0; k < 10; k++)
        {
            for(i = 0; i < 3; i++)
                niz[i] = rand() % 101;

            semop(pozitivanId, &sem_lock, 1);
            semop(mutexId, &sem_lock, 1);
            file = fopen("brojevi.dat", "a");
            //for(i = 0; i < 3; i++)
            fprintf(file, "proc1 -> %d\n", niz[0]);
            fclose(file);
            semop(mutexId, &sem_unlock, 1);
            semop(negativanId, &sem_unlock, 1);
        }

        exit(0);
    }
    else
    {
        pozitivanId = semget((key_t)POZIVAN_KEY, 1, 066);
        negativanId = semget((key_t)NEGATIVAN_KEY, 1, 066);
        mutexId = semget((key_t)MUTEX_KEY, 1, 066);

        for(k = 0; k < 10; k++)
        {
            data = rand() % 101;
            data = data - 2*data;

            semop(negativanId, &sem_lock, 1);
            semop(mutexId, &sem_lock, 1);
            file = fopen("brojevi.dat", "a");
            fprintf(file, "proc2 -> %d\n", data);
            fclose(file);
            semop(mutexId, &sem_unlock, 1);
            semop(pozitivanId, &sem_unlock, 1);
        }

        wait(&retCode);
        semctl(pozitivanId, 0, IPC_RMID, 0);
        semctl(negativanId, 0, IPC_RMID, 0);
        semctl(mutexId, 0, IPC_RMID, 0);

    }
}

























