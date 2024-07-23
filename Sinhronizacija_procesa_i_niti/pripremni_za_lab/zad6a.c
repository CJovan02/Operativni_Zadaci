/*Korišćenjem programskog jezika C napisati Linux progam koji se deli u dva procesa. Jedan proces čita
liniju po liniju datoteku prva.txt, a drugi datoteku druga.txt. Ova dva procesa upisuju naizmenično
pročitane linije u tekstualnu datoteku zbir.txt tako da su sve neparne linije iz datoteke prva.txt, a parne
iz datoteke druga.txt.*/
//RESENJE OVOG ZAD SAMO SA NITIMA!

//NE RADI NARAVNO
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semPrvi, semDrugi;
FILE* prva;
FILE* druga;
FILE* rez;

void* citajPrvi(void* arg)
{

    if(prva == NULL)
    {
        printf("Greska u otvaranju fajla prva.txt");
        exit(1);
    }
    char linija[50];
    printf("nit1 usla u kurac");

    while(!feof(prva));
    {
        fgets(linija, 50, prva);
        printf("Nit1 procitala: %s\n", linija);

        sem_wait(&semPrvi);
        rez = fopen("zad6Rezultat.txt", "w");
        fprintf(rez, "%s\n", linija);
        fclose(rez);
        sem_post(&semDrugi);
    }
    fclose(prva);
}

void* citajDrugi(void* arg)
{

    if(druga == NULL)
    {
        printf("Greska u otvaranju fajla druga.txt");
        exit(1);
    }
    char linija[50];
    printf("nit2 usla u kurac");

    while(!feof(druga));
    {
        fgets(linija, 50, druga);
        printf("Nit2 procitala: %s\n", linija);

        sem_wait(&semDrugi);
        rez = fopen("zad6Rezultat.txt", "w");
        fprintf(rez, "%s\n", linija);
        fclose(rez);
        sem_post(&semPrvi);
    }

    fclose(druga);
}

int main()
{
    printf("Uso sam u main");
    pthread_t nit1, nit2;

    prva = fopen("prva.txt", "r");
    druga = fopen("druga.txt", "r");
    sem_init(&semPrvi, 0, 1);
    sem_init(&semDrugi, 0, 0);
    printf("init sam semafore");
    pthread_create(&nit1, NULL, (void*)citajPrvi, NULL);
    printf("Napravio sam prvu nit");
    pthread_create(&nit2, NULL, (void*)citajDrugi, NULL);
    printf("Napravio sam drugu nit");

    pthread_join(nit1, NULL);
    pthread_join(nit2, NULL);

    sem_destroy(&semPrvi);
    sem_destroy(&semDrugi);

    return 0;
}















