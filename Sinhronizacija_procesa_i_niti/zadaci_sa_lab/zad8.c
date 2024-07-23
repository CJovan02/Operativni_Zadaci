/*Glavna nit kreira niz od N elemenata i popunjaga ga random od 0 do 10
  Glavna nit pokrece 2 dodate niti za sortiranje niza. Prva sortira prvu polovinu niza 0 - N/2-1
  u rastuci redosled, a druga nit drugu polovinu niza u opadajuci redosled n/2 - n -1, kada zavrse sa
  sortiranjem, glavna nit stampa niz. Koristiti mutex-e i uslovne promeljive*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N 10
#define TRUE 1
#define FALSE 0

int niz[N];

pthread_mutex_t mutex;
pthread_cond_t prvaPolPuna;
pthread_cond_t drugaPolPuna;
int prvaPuna = FALSE, drugaPuna = FALSE;


void* sortirajPrvuPolovinu(void* arg)
{
    int i, j;

    pthread_mutex_lock(&mutex);

    while(!prvaPuna)
        pthread_cond_wait(&prvaPolPuna, &mutex);

    for(i = 0; i < N/2 - 1; i++)
    {
        for(j = i; j < N/2; j++)
        {
            if(niz[i] > niz[j])
            {
                int pom = niz[i];
                niz[i] = niz[j];
                niz[j] = pom;
            }
        }
    }

    pthread_mutex_unlock(&mutex);
}

void* sortirajDruguPolovinu(void* arg)
{
    int i, j;

    pthread_mutex_lock(&mutex);

    while(!drugaPuna)
        pthread_cond_wait(&drugaPolPuna, &mutex);

    for(i = N/2 ; i < N - 1; i++)
    {
        for(j = i; j < N; j++)
        {
            if(niz[i] < niz[j])
            {
                int pom = niz[i];
                niz[i] = niz[j];
                niz[j] = pom;
            }
        }
    }

    pthread_mutex_unlock(&mutex);
}

int main()
{
    srand(time(NULL));
    pthread_t n1, n2;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&prvaPolPuna, NULL);
    pthread_cond_init(&drugaPolPuna, NULL);
    pthread_create(&n1, NULL, (void*)sortirajPrvuPolovinu, NULL);
    pthread_create(&n2, NULL, (void*)sortirajDruguPolovinu, NULL);

    int i, data;
    for(i = 0; i < N/2; i++)
    {
        data = rand() % 11;
        printf("%d ", data);
        niz[i] = data;
    }
    prvaPuna = TRUE;
    pthread_cond_signal(&prvaPolPuna);

    for(i = N/2; i < N; i++)
    {
        data = rand() % 11;
        printf("%d ", data);
        niz[i] = data;
    }
    drugaPuna = TRUE;
    pthread_cond_signal(&drugaPolPuna);

    pthread_join(n1, NULL);
    pthread_join(n2, NULL);

    printf("\nSortiran niz: \n");
    for(i = 0; i < N; i++)
        printf("%d ", niz[i]);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&prvaPolPuna);
    pthread_cond_destroy(&drugaPolPuna);
}



















