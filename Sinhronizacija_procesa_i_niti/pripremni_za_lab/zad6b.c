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
int mutexid, procaid, procbid, retCode;
union semun semopts;
char linija[DUZINA];
FILE* fileA;
FILE* fileB;
FILE* fileC;
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
fileA = fopen("prva.txt", "r");
while (!feof(fileA))
{
fgets(linija, DUZINA, fileA);
semop(procaid, &sem_lock, 1);
semop(mutexid, &sem_lock, 1);
fileC = fopen("zbir.txt", "a");
fprintf(fileC, "%s", linija);
fclose(fileC);
semop(mutexid, &sem_unlock, 1);
semop(procbid, &sem_unlock, 1);
}
fclose(fileA);
exit(0);
}
else
{
mutexid = semget((key_t)MUTEX_KEY, 1, 0666);
procaid = semget((key_t)PROC_A_KEY, 1, 0666);
procbid = semget((key_t)PROC_B_KEY, 1, 0666);
fileB = fopen("druga.txt", "r");
while (!feof(fileB))
{
fgets(linija, DUZINA, fileB);
semop(procbid, &sem_lock, 1);
semop(mutexid, &sem_lock, 1);
fileC = fopen("zbir.txt", "a");
fprintf(fileC, "%s", linija);
fclose(fileC);
semop(mutexid, &sem_unlock, 1);
semop(procaid, &sem_unlock, 1);
}
fclose(fileB);
wait(&retCode);
semctl(mutexid, 0, IPC_RMID, 0);
semctl(procaid, 0, IPC_RMID, 0);
semctl(procbid, 0, IPC_RMID, 0);
}
}
