/*
  Kreirati dodatni proces sa kojim komunicira dvosmerno korisnjecme datavoda. Roditeljski process
  generise 10 slucajnih brojeva u opsegu od 200 do 299 i salje ih putem datavoda procesu detetu. Proces
  dete roditeljskom procesu datavodom vraca samo brojeve deljive sa 3 i to uvecane za 25. roditeljskom
  te brojeve stampa na ekranu.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

#define N 10

int main()
{
    int pid, pd1[2], pd2[2];
    srand(time(0));

    if(pipe(pd1) == -1)
    {
        printf("Greska pri kreiranju prvog datavoda");
        return -1;
    }
    if(pipe(pd2) == -1)
    {
        printf("Greska pri kreiranju drugog datavoda");
        return -1;
    }

    if(pid = fork() != 0)
    {
        close(pd1[0]);
        close(pd2[1]);

        int niz[N], niz2[N], i, count;

        printf("Generisan niz: \n");
        for(i = 0; i < N; i++)
        {
            niz[i] = (rand() %  100) + 200;
            printf("!%d ", niz[i]);
        }
        printf("\n");

        write(pd1[1], niz, sizeof(int) * N);

        count = read(pd2[0], niz2, sizeof(int) * N);
        count = count / 4;

        printf("Modifikovan niz: \n");
        for(i = 0; i < count; i++)
            printf("#%d ", niz2[i]);
        printf("\n");


        close(pd2[0]);
        close(pd2[1]);
        wait(NULL);
    }
    else
    {
        close(pd1[1]);
        close(pd2[0]);

        int i, niz[N], niz2[N], j = 0;

        read(pd1[0], niz, sizeof(int) * N);

        for(i = 0; i < N; i++)
        {
            if(niz[i] % 3 == 0)
                niz2[j++] = niz[i] + 25;
        }

        printf("Niz koji se prenosi: \n");
        for(i = 0; i < j; i++)
            printf("@%d ", niz2[i]);
        printf("\n");

        write(pd2[1], niz2, sizeof(int) * j);

        close(pd1[0]);
        close(pd2[1]);
        exit(0);
    }
}



















