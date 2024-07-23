/*
 Glavni program sedelu u dva procesa. Prvi (proizvodjac) kreira N slucajnih pozitivnih brojeva
 i salje ih drugom procesu. Broj N unosi korisnik sa tastature. Po zavrsetku slanja, prvi proces salje -1 kao kod
 za kraj. Drugi proces (potrosac) preuzima poslate brojeve iz datavoda i upisuje ih u datoteku tmp.txt*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main()
{
    int pid, pd[2];
    srand(time(NULL));
    FILE *file;

    if(pipe(pd) == -1)
    {
        printf("Greska pri kreiranju datavoda\n");
        return -1;
    }

    if(pid = fork() == 0) //proizvodjac
    {
        close(pd[0]);
        //ne znam da li misle da se podaci salju jedan po jedan ili da se posalje ceo niz brojeva
        int n, data, i;
        char linija[80];

        gets(linija);
        n = atoi(linija);
        for(i = 0; i < n; i++)
        {
            data = rand() % 100;
            //printf("proizvodjac generisao: %d\n", data);
            write(pd[1], &data, sizeof(int));
        }
        int kodZaKraj = -1;
        write(pd[1], &kodZaKraj, sizeof(int));

        close(pd[1]);
        //ne znam na koji kod za kraj misle ali ne moze sigurne return -1 jer je to kod za gresku
        exit(0);
    }
    else //potrosac
    {
        file = fopen("tmp.txt", "w");
        close(pd[1]);

        int data, i, kodZaKraj;

        while(data != -1)
        {
            read(pd[0], &data, sizeof(int));
            if(data != -1)
            {
                fprintf(file, "%d ", data);
                printf("%d ", data);
            }
        }

        wait(NULL);
        close(pd[0]);
        fclose(file);
        return 0;
    }
}

















