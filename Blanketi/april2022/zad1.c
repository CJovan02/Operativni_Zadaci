/*
  Program prvo startuje drugu nit, a zatim u glavnoj niti od korisnika ocekuje da unese ceo broj.
  Drugoj niti prosledjuje uneti broj, a druga nit odbrojava (i stampa na ekranu brojeve kako odbrojava) od 0
  do unetog broja sa pauzom od 3 sekunde izmedju 2 uzastopna broja. zatim u glavnoj niti korissnik ponovo
  unosi ceo broj, prosledjuje ga drugoj niti koja odbrojava i ovaj proces odbrojavanja i unosa se ponavalja
  dok korisnik ne unese broj 99*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <pthread.h>

sem_t semUnos, semOdbrojavanje;
int broj;

void* odbroji(void* arg)
{
    int i;

    do
    {
        sem_wait(&semOdbrojavanje);

        if(broj != 99)
        {
            int count = 0;
            printf("Odbrojavanje: \n");
            for(i = 0; i <= broj; i++)
            {
                printf("%d\n", i);
                //sleep(3);
            }
            printf("\n");
        }

        sem_post(&semUnos);
    } while(broj != 99);
}

int main(int argc, char* argv[])
{
    pthread_t nit;

    sem_init(&semUnos, 0, 1);
    sem_init(&semOdbrojavanje, 0, 0);

    pthread_create(&nit, NULL, (void*)odbroji, NULL);

    char linija[80];

    do
    {
        sem_wait(&semUnos);

        printf("Unesite broj: ");
        gets(linija);
        broj = atoi(linija);

        sem_post(&semOdbrojavanje);
    } while(broj != 99);

    pthread_join(nit, NULL);
    sem_destroy(&semUnos);
    sem_destroy(&semOdbrojavanje);

    return 0;
}

















