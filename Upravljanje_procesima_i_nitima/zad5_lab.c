#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int pid;
    char datoteka[100];

    printf("Unesite ime datoteke koju zelie da kopirate u tmp folder: ");
    scanf("%s", datoteka);

    pid = fork();

    if(pid == 0)
    {
        if(execlp("cp", "cp", datoteka, "./tmp", NULL))
        {
            printf("Doslo je do greske! \n");
            exit(1);
        }
    }
    else
        wait(NULL);

    return 0;
}
