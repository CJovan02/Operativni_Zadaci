/*
  Program sadrzi bafer u koji se moze smestii 10 integer brojeva. Jedna dodatna nit generise 5 slucajnih brojeva
  od
  0 do 299 i upisuje ih na mesta sa parnim indeksima u ovaj bafer. Druga dodatna nit istovremeno generise 5
  slucajnih brojeva od 300 do 499 i upisuje ih na neparnim indeksima, glavna nit treba da odredi da li je zbir tih
  brojeva veci od 2000 i odstampa adekvatnu poruku na std izlazu. ovaj postupak generisanje 10 brojeva i
  isptitivanje vrednosti treba da se ponovi 5 puta. Sinhronizovati rad niti.*/

//NACIN SA KONDICIONALNIM PROMENLJIVAMA

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define N 10

int buffer[N];
pthread_mutex_t mutex1, mutex2;
pthread_cond_t paranPun, neparanPun, bufPrazan;
int paran, neparan, prazan1, prazan2;

void* nit1(void* arg)
{
    int i, k;

    for(k = 0; k < 5; k++)
    {
        pthread_mutex_lock(&mutex1);

        while(!prazan1)
            pthread_cond_wait(&bufPrazan, &mutex1);
        prazan1 = 0;

        for(i = 0; i < N - 1; i = i + 2)
            buffer[i] = rand() % 300;

        printf("nit1 upisala na parne pozicije, signalizira se main-u\n");
        paran = 1;
        pthread_cond_signal(&paranPun);

        pthread_mutex_unlock(&mutex1);
    }
}

void* nit2(void* arg)
{
    int i, k;

    for(k = 0; k < 5; k++)
    {
        pthread_mutex_lock(&mutex2);

        while(!prazan2)
            pthread_cond_wait(&bufPrazan, &mutex2);
        prazan2 = 0;

        for(i = 1; i < N; i = i + 2)
            buffer[i] = (rand() % 300) + 300;

        printf("nit2 upisala u neparne pozicije, signalizira se main-u\n");
        neparan = 1;
        pthread_cond_signal(&neparanPun);


        pthread_mutex_unlock(&mutex2);
    }
}

int main()
{
    srand(time(0));

    pthread_t n1, n2;
    int i, sum = 0, k;

    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);
    pthread_cond_init(&paranPun, NULL);
    pthread_cond_init(&neparanPun, NULL);
    pthread_cond_init(&bufPrazan, NULL);

    pthread_create(&n1, NULL, (void*)nit1, NULL);
    pthread_create(&n2, NULL, (void*)nit2, NULL);

    prazan1 = 1;
    prazan2 = 1;
    pthread_cond_broadcast(&bufPrazan);
    for(k = 0; k < 5; k++)
    {
        sum = 0;
        pthread_mutex_lock(&mutex1);

        while(!paran)
            pthread_cond_wait(&paranPun, &mutex1);
        paran = 0;

        for(i = 0; i < N - 1; i += 2)
            sum += buffer[i];
        printf("main sumira na parnim pozicijama\n");

        pthread_mutex_unlock(&mutex1);


        pthread_mutex_lock(&mutex2);

        while(!neparan)
            pthread_cond_wait(&neparanPun, &mutex2);
        neparan = 0;

        for(i = 1; i < N; i += 2)
            sum += buffer[i];
        printf("main sumira na neparnim pozicijama\n");

        pthread_mutex_unlock(&mutex2);

        if(sum > 2000)
            printf("Suma je veca od 2000, %d\n\n", sum);
        else
            printf("Suma je manja od 2000\n\n, %d", sum);

        sleep(3);

        prazan1 = 1;
        prazan2 = 1;
        pthread_cond_broadcast(&bufPrazan);
    }


    pthread_join(n1, NULL);
    pthread_join(n2, NULL);
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);
    pthread_cond_destroy(&paranPun);
    pthread_cond_destroy(&neparanPun);
    pthread_cond_destroy(&bufPrazan);
}




















