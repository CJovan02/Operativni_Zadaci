/*
 Kreirati dodatni process sa kojim komuicira koriscenjem datavoda. Roditeljski process generise 20 slucajnih
 brojeva u opsegu od 100 do 199 i salje ih putem datavoda procesu detetu. Proces dete na ekranu treba da odstampa samo
 brojeve koje dobije preko datavoda, a koji su deljivi sa 3. U slucaju broja koji nije deljiv sa 3 ne stampa
 nista.*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 20

int main()
{
    int pd[2], pid;
    srand(time(NULL));

    if(pipe(pd) == -1)
    {
        printf("Greska pri kreiranju datavoda!");
        return -1;
    }

    if(pid = fork() != 0) //roditelj
    {
        close(pd[0]);
        int data, i;
        for(i = 0; i < N; i++)
        {
            data = (rand() % 100) + 100;
            printf("%d ", data);
            write(pd[1], &data, sizeof(int));
        }
        printf("\n");

        close(pd[1]);
        wait(NULL);
        return 0;
    }
    else //dete
    {
        close(pd[1]);

        int data, i, count;

        for(i = 0; i < N; i++)
        {
            count = read(pd[0], &data, sizeof(int));
            if(count != 0 && (data % 3 == 0))
                printf("%d ", data);
        }

        close(pd[0]);
        exit(0);
    }
}





















