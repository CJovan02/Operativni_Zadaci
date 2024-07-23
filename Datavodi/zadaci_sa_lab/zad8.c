/*
  Kreirati dodatni process sa kojim se komunicira preko datavoda. Roditelj generise 20 slucajnih brojeva u opsegu od
  10 do 200 i salje ih putem datavoda procesu dete. Proces dete na ekranu treba da odstampa samo brojeve koje dobije
  preko datavoda a koji su deljivi sa 2. u slucaju broja koji nije deljiv sa 2 stampa se -1*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

#define N 20

int main()
{
    int pid, pd[2], i;
    srand(time(NULL));

    if(pipe(pd) == -1)
    {
        printf("Greska pri kreiranju datavoda");
        return -1;
    }

    if(pid = fork() != 0)
    {
        close(pd[0]);

        int data;
        printf("Generisan niz: \n");
        for(i = 0; i < N; i++)
        {
            data = (rand() % 191) + 10;
            write(pd[1], &data, sizeof(int));
            printf("%d ", data);
        }
        printf("\n");

        close(pd[1]);
        wait(NULL);
    }
    else
    {
        close(pd[1]);
        int data, count;

        printf("Rezultat: \n");
        for(i = 0; i < N; i++)
        {
            count = read(pd[0], &data, sizeof(int));
            if(count != 0)
            {
                if(data % 2 == 0)
                    printf("%d ", data);
                else
                    printf("-1 ");
            }
        }

        close(pd[0]);
        exit(0);
    }
}


















