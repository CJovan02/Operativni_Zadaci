/*Korišćenjem programskog jezika C napisati Linux program koji simulira komunikaciju između niti
korišćenjem celobrojnih bafera. Nit N0 generiše celobrojne podatke i upisuje ih u bafere B1, B2 i B3.
Prilikom upisivanja podataka, nit N0, najpre proverava da li u baferu B3 ima slobodnih pozicija i u tom
slučaju novi podatak upisuje u bafer B3. Ako je bafer B3 pun, proverava da li postoje slobodne pozicije
u baferu B1. Ako postoje, proizvođač N0 generisani podataka upisuje u bafer B1. Ukoliko je i bafer B1
pun novi podatak se upisuje u bafer B2 ukoliko on poseduje slobodne pozicije. Ukoliko ni u jednom
baferu nema slobodnih pozicija, nit N0 čeka, dok se u nekom od bafera ne oslobodi pozicija za upis
novog elementa. Niti N1, N2, N3 čitaju podatke iz bafera B1, B2, B3 i prikazje ih na standardnom
izlazu. Za sinhronizaciju između niti iskoristiti POSIX semafore*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define N1 10
#define N2 5
#define N3 20

int B1[N1], B2[N2], B3[N3];

int empty1 = N1;
int empty2 = N2;
int empty3 = N3;

int head1 = 0, tail1 = 0;
int head2 = 0, tail2 = 0;
int head3 = 0, tail3 = 0;


sem_t semB1, semB2, semB3, semEmpty;

void* nit0(void* arg)
{
    int data;

    while(1)
    {
        sleep(rand() % 3);
        //prvo se gleda da li postoji slobona pozicija u bilo kom baferu
        sem_wait(&semEmpty);
        data = rand() % 100;

        sem_wait(&semB3);
        if(empty3 > 0)
        {
            B3[tail3] = data;
            printf("nit0 upisala: B3[%d] = %d\n", tail3, data);
            tail3 = (tail3 + 1) % N3;
            empty1--;

            sem_post(&semB3);
            continue;
        }
        else
            sem_post(&semB3);

        sem_wait(&semB1);
        if(empty1 > 0)
        {
            B1[tail2] = data;
            printf("nit0 upisala: B1[%d] = %d\n", tail1, data);
            empty1--;
            tail2 = (tail2 + 1) % N2;

            sem_post(&semB1);
            continue;
        }
        else
            sem_post(&semB1);

        sem_wait(&semB2);
        if(empty2 > 0)
        {
            B2[tail3++] = data;
            printf("nit0 upisala: B2[%d] = %d\n", tail2, data);
            empty2--;
            tail1 = (tail1 + 1) % N1;

            sem_post(&semB1);
            continue;
        }
        else
            sem_post(&semB1);
    }

}

void* nit1(void* arg)
{
    while(1)
    {
        sem_wait(&semB1);

        if(empty1 < N1);
        {
            int data = B1[head1];
            empty1++;
            printf("N1 procitala, B1[%d] = %d\n", head1, data);
            head1 = (head1 + 1) % N1;
            sem_post(&semEmpty);
        }
        sem_post(&semB1);

        sleep(rand() % 5);
    }
}

void* nit2(void* arg)
{
    while(1)
    {
        sem_wait(&semB2);

        if(empty2 < N2);
        {
            int data = B2[head2];
            empty2++;
            printf("N2 procitala, B2[%d] = %d\n", head2, data);
            head2 = (head2 + 1) % N2;
            sem_post(&semEmpty);
        }
        sem_post(&semB2);

        sleep(rand() % 7);
    }
}

void* nit3(void* arg)
{
    while(1)
    {
        sem_wait(&semB3);

        if(empty3 < N3);
        {
            int data = B3[head3];
            empty3++;
            printf("N3 procitala, B3[%d] = %d\n", head3, data);
            head3 = (head3 + 1) % N3;
            sem_post(&semEmpty);
        }
        sem_post(&semB3);

        sleep(rand() % 9);
    }
}


int main()
{
    srand(time(NULL));

    pthread_t niti[4];

    sem_init(&semB1, 0, 1);
    sem_init(&semB2, 0, 1);
    sem_init(&semB3, 0, 1);
    sem_init(&semEmpty, 0, N1 + N2 + N3);


    pthread_create(&niti[1], NULL, (void*)nit1, NULL);
    pthread_create(&niti[2], NULL, (void*)nit2, NULL);
    pthread_create(&niti[3], NULL, (void*)nit3, NULL);
    pthread_create(&niti[0], NULL, (void*)nit0, NULL);

    int i;
    for(i = 0; i < 4; i++)
        pthread_join(niti[i], NULL);

    sem_destroy(&semB1);
    sem_destroy(&semB2);
    sem_destroy(&semB3);
    sem_destroy(&semEmpty);

}



















