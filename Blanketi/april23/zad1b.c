/*
  Program sadrzi bafer u koji se moze smestii 10 integer brojeva. Jedna dodatna nit generise 5 slucajnih brojeva
  od
  0 do 299 i upisuje ih na mesta sa parnim indeksima u ovaj bafer. Druga dodatna nit istovremeno generise 5
  slucajnih brojeva od 300 do 499 i upisuje ih na neparnim indeksima, glavna nit treba da odredi da li je zbir tih
  brojeva veci od 2000 i odstampa adekvatnu poruku na std izlazu. ovaj postupak generisanje 10 brojeva i
  isptitivanje vrednosti treba da se ponovi 5 puta. Sinhronizovati rad niti.*/

//NACIN SA SEMAFORIMA

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define N 10

int buffer[N];
sem_t semParan, semNeparan, semParanPrazan, semNeparanPrazan;

void* nit1(void* arg)
{
    int i, k;

    for(k = 0; k < 5; k++)
    {
        sem_wait(&semParan);

        for(i = 0; i < N - 1; i += 2)
            buffer[i] = rand() % 300;
        printf("nit1 generisala parne indexe\n");

        sem_post(&semParanPrazan);
    }
}

void* nit2(void* arg)
{
    int i, k;

    for(k = 0; k < 5; k++)
    {
        sem_wait(&semNeparan);

        for(i = 1; i < N; i += 2)
            buffer[i] = (rand() % 200) + 300;

        printf("nit2 generisala neparne indexe\n");

        sem_post(&semNeparanPrazan);
    }
}

int main()
{
    pthread_t n1, n2;
    srand(time(0));
    int sum = 0, i, k;

    sem_init(&semParan, 0, 1);
    sem_init(&semNeparan, 0, 1);
    sem_init(&semParanPrazan, 0, 0);
    sem_init(&semNeparanPrazan, 0, 0);

    pthread_create(&n1, NULL, (void*)nit1, NULL);
    pthread_create(&n2, NULL, (void*)nit2, NULL);

    for(k = 0; k < 5; k++)
    {
        sum = 0;
        sem_wait(&semParanPrazan);

        for(i = 0; i < N - 1; i += 2)
            sum += buffer[i];
        printf("Main sumirao parne indexe\n");

        sem_wait(&semNeparanPrazan);

        for(i = 1; i < N; i += 2)
            sum += buffer[i];
        printf("Main sumirao neparne indexe\n");

        printf("%d\n\n\n", sum);

        sleep(3);

        sem_post(&semParan);
        sem_post(&semNeparan);
    }


    pthread_join(n1, NULL);
    pthread_join(n2, NULL);
    sem_destroy(&semParan);
    sem_destroy(&semNeparan);
    sem_destroy(&semParanPrazan);
    sem_destroy(&semNeparanPrazan);
}












