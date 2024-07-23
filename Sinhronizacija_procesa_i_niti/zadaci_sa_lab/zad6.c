/*Koriscenjem programskog jezika C kreirati Linux program koji na standardnom
 izlazu stampa velika i mala slova abeceda u sledecom redosledu:aAbBcC...zZ
 Pri tome mala slova stampa jedna nit, a velika slova durga nit.
 Sinhronizovati niti tako da se slova abeceda na ekranu stampaju pravlinim
 redosledom*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define POC_VELIKA 65
#define KRAJ_VELIKA 90
#define POC_MALA 97
#define KRAJ_MALA 122

sem_t semMala, semVelika;

void* stampajMala(void* arg)
{
	int i;
	for(i = POC_VELIKA; i <= KRAJ_VELIKA; i++)
	{
		sem_wait(&semVelika);
		printf("%c", i);
		sem_post(&semMala);
	}
}

void* stampajVelika(void* arg)
{
	int i;
        for(i = POC_MALA; i <= KRAJ_MALA; i++)
        {
                sem_wait(&semMala);
                printf("%c", i);
                sem_post(&semVelika);
        }
}

int main()
{
 pthread_t nit1, nit2;

 sem_init(&semMala, 0, 1);
 sem_init(&semVelika, 0, 0);

 pthread_create(&nit1, NULL, (void*)stampajVelika, NULL);
 pthread_create(&nit2, NULL, (void*)stampajMala, NULL);

 pthread_join(nit1, NULL);
 pthread_join(nit2, NULL);

 sem_destroy(&semMala);
 sem_destroy(&semVelika);

 return 0;
}
