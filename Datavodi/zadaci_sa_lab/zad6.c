/*
 Prvi proces generise random char od a-z, a drugi proces ga prima i upisuje u chars.txt, komunikacija se ponavlja
 25 puta.*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define N 25

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

        //od 97 do 122
        char data;
        for(i = 0; i < N; i++)
        {
            data = (rand() % 26) + 97;
            write(pd[1], &data, sizeof(char));
        }

        close(pd[1]);
        wait(NULL);
        return 0;
    }
    else
    {
        close(pd[1]);
        int count;
        char data;

        for(i = 0; i < N; i++)
        {
            count = read(pd[0], &data, sizeof(char));
            if(count != 0)
            {
                //mrzi me da upisem u fajl
                printf("%c ", data);
            }
        }

        close(pd[0]);
        exit(0);
    }
}































