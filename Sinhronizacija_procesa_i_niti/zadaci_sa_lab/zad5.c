/*
  Dve niti u txt upisuju redom cele brojeve od 1 do 20, pri cemu je svaki broj u zasebnom redu.
  Prva nit upisuje samo neparne, a druga nit samo parne brojeve. Sinhronizacijom niti obezbediti
  da se brojevi upisuju u datoteku prvailnim redosledom, 1, 2, 3, 4, 5 itd.*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semParni, semNeparni;
FILE *file;

void* upisiNeparan(void* arg)
{
    int i;
    for(i = 1; i <= 19; i = i + 2)
    {
        sem_wait(&semNeparni);
        if(file == NULL)
        {
            printf("Greska u otvaranju fajla\n");
            return;
        }
        fprintf(file, "n2 - %d\n", i);
        sem_post(&semParni);
    }
}

void* upisiParan(void* arg)
{
    int i;
    for(i = 0; i <= 20; i = i + 2)
    {
        sem_wait(&semParni);
        if(file == NULL)
        {
            printf("Greska u otvaranju fajla\n");
            return;
        }
        fprintf(file, "n1 - %d\n", i);
        sem_post(&semNeparni);
    }
}

int main()
{
    pthread_t n1, n2;

    file = fopen("zad5Rezultat.txt", "w");
    sem_init(&semParni, 0, 1);
    sem_init(&semNeparni, 0, 0);
    pthread_create(&n1, NULL, (void*)upisiParan, NULL);
    pthread_create(&n2, NULL, (void*)upisiNeparan, NULL);

    pthread_join(n1, NULL);
    pthread_join(n2, NULL);
    sem_destroy(&semParni);
    sem_destroy(&semNeparni);

    return 0;
}



























