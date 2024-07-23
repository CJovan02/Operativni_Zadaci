/*
  Napisati Linux program koji iz txt datoteke ulaz.txt ucitava niz celih brojeva (svaki broj u zasebnoj
  liniji) u niz u memoriji. Glavni program odmah po startovanju kreira dodatnu nit koja sortira brojeve
  ucitane u niz u memoriji. Po zavrsetku sortiranja glavni program stampa sortirani niz.*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0

pthread_mutex_t mutex;
pthread_cond_t ucitanNiz;
int ucitan;

int n, niz[100];

void* sortiraj(void* arg)
{
    int i, j, pom;

    pthread_mutex_lock(&mutex);

    while(!ucitan)
        pthread_cond_wait(&ucitanNiz, &mutex);
    ucitan = FALSE;

    for(i = 0; i < n - 1; i++)
    {
        for(j = i; j < n; j++)
        {
            if(niz[i] > niz[j])
            {
                pom = niz[i];
                niz[i] = niz[j];
                niz[j] = pom;
            }
        }
    }

    pthread_mutex_unlock(&mutex);
}

int main()
{
    int i;
    n = 0;
    FILE *file = fopen("ulaz.txt", "r");
    char linija[2];
    pthread_t nit;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&ucitanNiz, NULL);

    pthread_create(&nit, NULL, (void*)sortiraj, NULL);

    pthread_mutex_lock(&mutex);

    printf("Ucitan niz: \n");
    while(!feof(file))
    {
        fscanf(file, "%d", &niz[n]);
        printf("%d ", niz[n]);
        n++;
    }
    fclose(file);

    ucitan = TRUE;
    pthread_cond_signal(&ucitanNiz);

    pthread_mutex_unlock(&mutex);


    pthread_join(nit, NULL);

    printf("\nSortiran niz: \n");
    for(i = 0; i < n; i++)
        printf("%d ", niz[i]);


    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&ucitanNiz);
}
















