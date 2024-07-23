/*
 Napraviti tri Linux rpcesa koja komuniciraju koriscenjem datavoda. Prvi proces generise niz proizvoljih
 karaktera (duzina niza je fiksna i iznosi 20 karaktera) a zatim racuna zbir ASCII vrednosti svih tako generisanih
 karaktera. Ukoliko je dobijeni zbir paran broj, generisani niz char se datavodom salje drugom procesu koji ih
 upisuje u datoteku KARAKTERI.txt, a ako je zbir neparan broj, stampa se na standardnom izlazu*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define N 20

int main()
{
    int pd1[2], pd2[2], pid1, pid2, i;
    srand(time(NULL));

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

    if(pid1 = fork() != 0) //prvi process
    {
        close(pd1[0]);
        close(pd2[0]);

        char niz[N];
        int sum = 0;
        //od 33 do 126
        for(i = 0; i < N; i++)
        {
            niz[i] = (char)(rand() % 94) + 33;
            sum += niz[i];
            printf("%d\t%c\n", niz[i], niz[i]);
        }
        printf("sum: %d\n", sum);

        if(sum % 2 == 0)
            write(pd1[1], niz, N);
        else
            write(pd2[1], niz, N);

        close(pd1[1]);
        close(pd2[1]);
        wait(NULL);
        return 0;
    }
    else if(pid2 = fork() != 0) //drugi process
    {
        //ako je paran zbir upisuje ih u fajl KARAKTERI.txt
        close(pd2[0]);
        close(pd2[1]);
        close(pd1[1]);

        char niz[N];
        int count;

        count = read(pd1[0], niz, N);
        printf("%d\n", count);
        if(count > 0)
        {
            FILE *file = fopen("KARAKTERI.txt", "w");
            printf("Zbir je paran broj, niz se upisuje u fajl KARAKTERI.txt\n");
            for(i = 0; i < N; i++)
                fprintf(file, "%c\n", niz[i]);
            fclose(file);
        }

        close(pd1[0]);
        exit(0);
    }
    else //treci process
    {
        close(pd1[0]);
        close(pd1[1]);
        close(pd2[1]);
        //ako je neparan zbir ispisuje ih na konzolu

        char niz[N];
        int count;

        count = read(pd2[0], niz, N);
        printf("%d\n", count);
        if(count > 0)
        {
            printf("Zbir je nepran broj, niz se ispisuje u konzoli: \n");
            for(i = 0; i < N; i++)
                printf("%c\n", niz[i]);
        }

        close(pd2[0]);
        exit(0);
    }
}


























