#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0

int buffer[2];

pthread_mutex_t mutex;
pthread_cond_t condVarFull;
pthread_cond_t condVarEmpty;
int bufferFull = FALSE;

void* fillBuffer(void* args)
{
    long int rand;
    float normalisedRand;
    int sleepTime;
    int i;
    int bufferPointer = 0;

    for(i = 1; i <=10; i++)
    {
        rand = random();
        normalisedRand = (float)rand/(float)RAND_MAX;
        sleepTime = (int)(normalisedRand * 5);

        pthread_mutex_lock(&mutex);
        while(bufferFull)
            pthread_cond_wait(&condVarEmpty, &mutex);
        buffer[bufferPointer] = i;
        printf("Trhead u buffer[%d] upisao vrednost %d\n", bufferPointer, i);
        bufferPointer = (bufferPointer + 1) % 2;
        if(bufferPointer == 0)
            bufferFull = TRUE;

        pthread_cond_signal(&condVarFull);
        pthread_mutex_unlock(&mutex);
        sleep(sleepTime);
    }
}

int main()
{
    pthread_t threadID;
    int i;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condVarEmpty, NULL);
    pthread_cond_init(&condVarFull, NULL);
    pthread_create(&threadID, NULL, (void*)fillBuffer, NULL);

    for(int i = 0; i < 5; i++)
    {
        pthread_mutex_lock(&mutex);
        while(!bufferFull)
            pthread_cond_wait(&condVarFull, &mutex);

        printf("%d + %d = %d \n", buffer[0], buffer[1], buffer[0] + buffer[1]);
        bufferFull = FALSE;
        pthread_cond_signal(&condVarEmpty);
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condVarEmpty);
    pthread_cond_destroy(&condVarFull);

    return 0;
}
