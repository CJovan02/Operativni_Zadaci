#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//sortiranje matrice samo sto svaka nit sortira svoju vrstu

#define N 3
int a[N][N];

//f-ja koju izvrsava nit
void* sortiranje(void* arg)
{
    //ulazni argument je vrsta matrice koja treba da se sortira
    //samo sto je ulazni arg void* pa mora da se castuje u int*
    int k = *((int*)arg);
    int pom, i, j;

    printf("Sortiranje %d vrste:\n", k);
    for(i = 0; i < N - 1; i++)
    {
        for(j = i + 1; j < N; j++)
        {
            if(a[k][i] > a[k][j])
            {
                pom = a[k][i];
                a[k][i] = a[k][j];
                a[k][j] = pom;
            }
        }
    }
}

int main()
{
    pthread_t niti[N];
    int i, j;

    for(i = 0; i < N; i++)
        for(j = 0; j < N; j++)
            scanf("%d", &a[i][j]);

    //stamapnje matrice
    printf("Inicijalna matrica: \n");
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
            printf("%d ", a[i][j]);
        printf("\n");
    }

    //kreiranje niti
    for(i = 0; i < N; i++)
        pthread_create(&niti[i], NULL, (void*)sortiranje, (void*)&i);

    //main f-ja ceka da se niti zavrse
    for(i = 0; i < N; i++)
        pthread_join(niti[i], NULL);

    //stampanje sortiranje matrice
    printf("Sortirana matrica: \n");
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < N; j++)
            printf("%d ", a[i][j]);
        printf("\n");
    }
}
