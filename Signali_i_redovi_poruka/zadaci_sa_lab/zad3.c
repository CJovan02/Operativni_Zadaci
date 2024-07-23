/*Koriscenjem programskog jezika C napisati Linux program koji omogucava da dva procesa komuniciraju
 i sinhronizuju svoje izvrsenje koriscenjem signala. Prvi proces po slucajnom principu kreira 1024 
 celobrojne vrednosti i smesta ih u binarnu datoteku prenos.dat. Kada izgenerise brojeve, 
 obavestava o tome drugi proces, slanjem signala SIGUSR1 i pauzira svoje izvrsavanje. 
 Kada drugi proces primi signal on sadrzaj datoteke prenos.dat prikazuje na standardnom izlazu, 
 salje prvom procesu signal SIGUSR2 i nakon toga pauzira svoje izvrsavanje. Po prijemu signala SIGUSR2, 
 prvi proces ponovo startuje citav postupak. Postupak se ponavlja 4096 puta. Nakon toga se oba procesa zavrsavaju.*/

//problem proizvodjac potrosac, u zadatku je objasnjen nacin za sinhronizaciju ovih procesa uz pomoc pause() f-je
//ne radi kako treba zadatak :D

 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <signal.h>
 #include <time.h>
 #include <sys/wait.h>

 FILE* file;

void generisi()
{
    int i, broj;
    file = fopen("prenos.txt", "w");

    for(i = 0; i < 10; i++)
    {
        broj = rand() % 1001;
        fprintf(file, "%d\n", broj);
    }

    fclose(file);
}

void citaj()
{
    int i, broj;
    char linija[50];
    file = fopen("prenos.txt", "r");

    for(i = 0; i < 10; i++)
    {
        fscanf(file, "%d", &broj);
        printf("%d\n", broj);
    }
    printf("\n\n");

    fclose(file);
}

 void main()
 {
    srand(time(NULL));

    int pid;

    pid = fork();

    if(pid != 0)
    {
        int i;
        printf("parent process: pid deteta: %d, pid parenta: %d\n", pid, getpid());

        //definise se sta radi proces kada mu stigne signal SIGUSR2
        signal(SIGUSR2, generisi);
        
        for(i = 0; i < 10; i++)
        {
            pause();
            kill(pid, SIGUSR1);
        }

        wait(NULL);
        exit(0);
    }
    else
    {
        int i;
        printf("child process: pid deteta: %d, pid parenta: %d\n", getpid(), getppid());

        //definise se sta radi proces kada mu stigne signal SIGUSR1
        signal(SIGUSR1, citaj);

        for(i = 0; i < 10; i++)
        {
            kill(getppid(), SIGUSR2);
            pause();
        }

        exit(0);
    }
 }