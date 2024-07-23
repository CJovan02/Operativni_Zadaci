//recenicu ispisuju posebno niti i sinhronizavone su koriscenjem semafora

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5

char poruka[N][20] = {"Ovo ", "je ", "test ", "za ", "semaphore."};

sem_t sems[N-1];

void* stampanje(void* args)
{
    int p = *((int*)args);
    //p - 1 zato sto za prvu nit nemamo semaphore pa nit sa indeksom 1 odgorvara sem sa indeksom 0 itd..
    //znaci svaka nit "se blokira" i ceka da je prethodna nit odblokira
    if(p > 0)
        sem_wait(&sems[p - 1]);

    printf("%s", poruka[p]);

    if(p < N - 1)
        sem_post(&sems[p]);

    sleep(1);
}

int main()
{
    int args[N];
    pthread_t niti[N];

    int i;
    for(i = 0; i < N-1; i++)
        sem_init(&sems[i], 0, 0);

    for(i = 0; i < N; i++)
    {
        args[i] = i;
        pthread_create(&niti[i], NULL, (void*)stampanje, (void*)&args[i]);
    }
    for(i = 0; i < N; i++)
        pthread_join(niti[i], NULL);

    for(i = 0; i < N; i++)
        sem_destroy(&sems[i]);
    return 0;
}
