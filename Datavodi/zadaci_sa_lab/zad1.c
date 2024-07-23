/*
 Kreirati tri Linux procesa koji komunicijraju koriscenjem datavoda. Prvi poces generise niz porizvoljnih
 celih brojeva (duzina niza je fiksna = 15 karaktera). Ukoliko je prvi generisani broj paran, generisani niza
 brojeva se koriscenjem datavoda salje drugom procesu koji ih upisuje u datoteku PARNI.txt.
 Ukoliko je prvi generisani broj neparan, generiani niz brojeva se koriscenjem datavoda salje trecem procesu koji
 ih stampa na standardnom izlazu*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main()
{
    int pd1[2], pd2[2], pid1, pid2;
    int niz[15], i;
    FILE *file;
    srand(time(NULL));

    if(pipe(pd1) == -1)
    {
        printf("Greska pri kreiranju prvog datavoda\n");
        return -1;
    }
    if(pipe(pd2) == -1)
    {
        printf("Greska pri kreiranju drugog datavoda\n");
        return -1;
    }

    if(pid1 = fork() != 0) //1. proces
    {
        close(pd1[0]);
        close(pd2[0]);
        int count;
        int obrada;
        printf("Generisani niz: \n");
        for(i = 0; i < 15; i++)
        {
            niz[i] = rand() % 100;
            printf("%d ", niz[i]);
        }
        printf("\n");

        if(niz[0] % 2 == 0) //ako je niz[0] paran, salje se 2. procesu
        {
            write(pd1[1], niz, sizeof(int) * 15);
        }
        else //ako je niz[0] neparan, salje se 3. procesu
        {
            write(pd2[1], niz, sizeof(int) * 15);
        }


        close(pd2[1]);
        close(pd1[1]);
        wait(NULL);
        return 0;
    }
    else if(pid2 = fork() != 0) //2. proces
    {
        //upisuje niz u PARNI.txt
        int result[15];
        int obrada, count;
        close(pd2[0]);
        close(pd2[1]);
        close(pd1[1]);

        count = read(pd1[0], result, sizeof(int) * 15);
        printf("count: %d \n", count);

        if(count != 0)
        {
            printf("niz[0] je paran, niz upisan u fajl\n");
            file = fopen("PARNI.txt", "w");
            for(i = 0; i < 15; i++)
                fprintf(file, "%d ", result[i]);

            fclose(file);
        }


        close(pd1[0]);
        wait(NULL);
        exit(0);
    }
    else //3. proces
    {
        //ispisuje niz na konzolu
        int result[15];
        int obrada, count;
        close(pd1[0]);
        close(pd1[1]);
        close(pd2[1]);

        count = read(pd2[0], result, sizeof(int) * 15);
        printf("count: %d \n", count);

        if(count != 0)
        {
            printf("niz[0] je neparan, niz se ispisuje na konzolu: \n");

            for(i = 0; i < 15; i++)
                printf("%d ", result[i]);
        }



        close(pd2[0]);
        exit(0);
    }
}














