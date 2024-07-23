/*Program sadrzi bafer u koji se moze smestiti 10 integer brojeva. Jedna zasebna nit, kreirana prilikom startovanja
programa, periodicno generise 10 slucajnih brojeva (u opsegu od 0 do 199) i upisuje ih u ovaj bafer. Kada se u
baferu nadje nova grupa brojeva, glavna nit (main funkcija) treba da odredi da li je zbir tih brojeva veci od 800 i
odstampa adekvatnu poruku na standardnom izlazu. Ovaj postupak generisanja 10 brojeva i ispitivanje vrednosti
njihovog zbira treba da se ponavalja sve dok sracunat zbir ne predje 1000. Adekvatno sinhronizovati rad niti koja
generise brojeve i glavne niti koja proverava njihov zbir*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define N 10

sem_t sem_proizvodjac, sem_potrosac;
int buffer[N], uslov;

void* generisiBrojeve(void* arg)
{
    int i;

    while(uslov == 1)
    {
        sem_wait(&sem_proizvodjac);

        for(i = 0; i < N; i++)
            buffer[i] = rand() % 200;

        sleep(1);

        sem_post(&sem_potrosac);

    }
}

int main()
{
    srand(time(0));
    int i, sum = 1;
    uslov = 1;

    pthread_t nit;
    sem_init(&sem_proizvodjac, 0, 1);
    sem_init(&sem_potrosac, 0, 0);

    pthread_create(&nit, NULL, (void*)generisiBrojeve, NULL);

    while(uslov == 1)
    {
        sem_wait(&sem_potrosac);

        sum = 0;

        for(i = 0; i < N; i++)
            sum += buffer[i];

        if(sum > 800)
        {
            printf("Suma je %d, i veca je od 800\n", sum);

            if(sum > 1000)
                uslov = 0;
        }
        else
            printf("Suma je %d, i manja je ili jednaka 800\n", sum);

        //sleep(1);

        sem_post(&sem_proizvodjac);
    }

    pthread_join(nit, NULL);
    sem_destroy(&sem_proizvodjac);
    sem_destroy(&sem_potrosac);

    return 0;
}


















