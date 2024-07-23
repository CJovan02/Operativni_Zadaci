#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 9
char poruka[N][20] = {"Ovo ", "je ", "test ", "za ", "threadove", "J", "+", "E", "<3"};

struct argument
{
    int rec;
    pthread_t prev;
};

void* stampaj(void* arg)
{
    struct argument *p = (struct argument*)arg;

    if(p->rec > 0)
    {
        //printf("Cekam na prethodnu nit");
        pthread_join(p->prev, NULL);
    }
    //printf("%d", p->rec);
    printf("%s \n", poruka[p->rec]);
    sleep(1);
}

int main()
{
    int i;
    struct argument args[N];
    pthread_t niti[N];

    for(int i = 0; i < N; i++)
    {
        args[i].rec = i;
        if(i > 0)
            args[i].prev = niti[i - 1];
        //printf("Kreiram %d nit, rec je: %s\n", i, poruka[i]);
        pthread_create(&niti[i], NULL, (void*)stampaj, (void*) &args[i]);
    }

    pthread_join(niti[N - 1], NULL);

    return 0;
}
