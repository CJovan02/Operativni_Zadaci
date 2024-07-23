/*Imamo niz od 20 elemenata i 3 niti, n1 na svake 2 sekunde upisuje random broj na random
  poziciju u prvoj polovini niza, n2 na 4 sekunde upisuje random broj na random poziciju
  u drugoj polovini niza, a n3 na 8 sekunde upise random broj na random poziciju u nizu
  Niti treba sinhronizovati nekom od poznatih metoda*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define N 20

int arr[N];

pthread_mutex_t mut1, mut2;

void* nit1(void* arg)
{
    srand(time(NULL));
    //index mora da bude izmedju 0 i 9
    while(1)
    {
        int index = rand() % 10;
        int number = rand() % 100;
        pthread_mutex_lock(&mut1);

        arr[index] = number;
        printf("Nit N1 upisala sledece: arr[%d] = %d\n", index, number);

        pthread_mutex_unlock(&mut1);
        sleep(2);
    }
}

void* nit2(void* arg)
{
    //index mora da bude izmedju 10 i 19
    while(1)
    {
        int index = (rand() % 10) + 10;
        int number = rand() % 100;
        pthread_mutex_lock(&mut2);

        arr[index] = number;
        printf("Nit N2 upisala sledece: arr[%d] = %d\n", index, number);

        pthread_mutex_unlock(&mut2);
        sleep(4);
    }
}

void* nit3(void* arg)
{
    //index mora da bude izmedju 0 i 19
    while(1)
    {
        int index = rand() % 20;
        int number = rand() % 100;
        //nit3 moze da zakljuca oba mutexa, ili da se vidi da li je index iz opsega prve polovine niza
        //i onda da zakljuca samo mutex1, ili iz druge polovine i da zakljuca mutex2
        if(index <= 9)
        {
            pthread_mutex_lock(&mut1);

            arr[index] = number;
            printf("Nit N3 upisala sledece: arr[%d] = %d\n", index, number);

            pthread_mutex_unlock(&mut1);
        }
        else
        {
            pthread_mutex_lock(&mut2);

            arr[index] = number;
            printf("Nit N3 upisala sledece: arr[%d] = %d\n", index, number);

            pthread_mutex_unlock(&mut2);
        }


        sleep(8);
    }
}

int main()
{
    pthread_t n1, n2, n3;

    pthread_mutex_init(&mut1, NULL);
    pthread_mutex_init(&mut2, NULL);

    pthread_create(&n1, NULL, (void*)nit1, NULL);
    pthread_create(&n2, NULL, (void*)nit2, NULL);
    pthread_create(&n3, NULL, (void*)nit3, NULL);

    pthread_join(n1, NULL);
    pthread_join(n2, NULL);
    pthread_join(n3, NULL);

    pthread_mutex_destroy(&mut1);
    pthread_mutex_destroy(&mut2);

    return 0;
}












