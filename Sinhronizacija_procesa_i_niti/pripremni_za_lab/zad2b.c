//drugi nacin za ovaj zadatak
/*Korišćenjem programskog jezika C napisati Linux progam koji sadrži bafer u koji se mogu smestiti dva
integer broja. Zasebna nit periodično generiše dva broja (u opsegu od 1 do 10) i upisuje ih u ovaj bafer.
Kada se u baferu nađe novi par brojeva glavna nit (main funkcija) treba da odredi zbir ta dva broja i
odštampa ga na standardnom izlazu. Vremenski razmak između dva uzastopna upisa u bafer je slučajna
vrednost između 0 i 5 sekundi*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

int buffer[2];

pthread_mutex_t mutex;
pthread_cond_t buffFull;
pthread_cond_t buffEmpty;

int isFull = 0;

void* fillBuffer(void* arg)
{
    srand(time(NULL));
    int rand1, rand2, rand3;
    int i;
    for(i = 0; i < 10; i++)
    {
        rand1 = (rand() % 9) + 1;
        rand2 = (rand() % 9) + 1;
        rand3 = rand() % 5;

        pthread_mutex_lock(&mutex);
        while(isFull)
            pthread_cond_wait(&buffEmpty, &mutex);
        buffer[0] = rand1;
        buffer[1] = rand2;
        printf("u bafer upisane vrednost %d i %d\n", buffer[0], buffer[1]);
        isFull = TRUE;

        pthread_cond_signal(&buffFull);
        pthread_mutex_unlock(&mutex);
        sleep(rand3);
    }
}

int main()
{
    pthread_t thread;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&buffEmpty, NULL);
    pthread_cond_init(&buffFull, NULL);
    pthread_create(&thread, NULL, (void*)fillBuffer, NULL);

    int i;
    for(i = 0; i < 10; i++)
    {
        pthread_mutex_lock(&mutex);
        while(!isFull)
            pthread_cond_wait(&buffFull, &mutex);

        isFull = FALSE;
        printf("%d + %d = %d\n", buffer[0], buffer[1], buffer[0] + buffer[1]);
        pthread_cond_signal(&buffEmpty);
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&buffEmpty);
    pthread_cond_destroy(&buffFull);

    return 0;
}






















