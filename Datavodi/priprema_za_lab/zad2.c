/*
  Zadatak 1
Korišćenjem programskog jezika C napisati Linux program koji kreira dva dodatna procesa. Proces
roditelj i ova dva novokreirana procesa deteta su povezana datavodima u red. Proces roditelj
prihvata rečenicu po rečenicu koje korisnik unosi sa tastature. Korišćenjem datavoda proces roditelj
unetu rečenicu prosleđuje prvom procesu detetu. Ovaj proces proverava da li je prvi karakter u
rečenici veliko slovo i ukoliko nije konvertuje ga u odgovarajuće veliko slovo. Rečenicu zatim,
koristeći datavod, šalje drugom procesu detetu. Naredni proces proverava da li je poslednji karakter
u rečenici tačka (.). Ukoliko nije dodaje tačku i rečenicu štampa na standardnom izlazu.

 Zadatak 2
Modofikovati rešenje iz prethodnog zadatka takod a su tri procesa datavodima povezani u krug.
Drugi proces dete ne štampa rečenicu već je datavodom šalje procesu roditelju koji je onda štampa
na standardni izlaz bez ikakve modifikacije.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define N 80

int main()
{
    int pid1, pid2, pd1[2], pd2[2], pd3[2];

    if(pipe(pd1) == -1)
    {
        printf("Greska pri kreiranju datavoda");
        return -1;
    }
    if(pipe(pd2) == -1)
    {
        printf("Greska pri kreiranju datavoda");
        return -1;
    }
    if(pipe(pd3) == -1)
    {
        printf("Greska pri kreiranju datavoda");
        return -1;
    }

    if(pid1 = fork() != 0)
    {
        close(pd2[0]);
        close(pd2[1]);

        close(pd3[1]);

        close(pd1[0]);

        char line[N], line2[N];
        do
        {
            gets(line);
            write(pd1[1], line, strlen(line) + 1);

            read(pd3[0], line2, N);
            printf("proc1 -> %s\n", line2);

        }while(strcmp(line, "KRAJ") != 0);

        wait(NULL);
        close(pd1[1]);
        close(pd3[0]);
    }
    else if(pid2 = fork() != 0)
    {
        close(pd1[1]);
        close(pd2[0]);
        close(pd3[0]);

        char line[N], line2[N];
        do
        {
            read(pd1[0], line, N);
            write(pd3[1], line, strlen(line) + 1);

            line[0] = toupper(line[0]);

            write(pd2[1], line, strlen(line) + 1);
            //printf("proc2 -> %s\n", line);

        }while(strcmp(line, "KRAJ") != 0);

        close(pd3[1]);
        close(pd2[1]);
        close(pd1[0]);
        exit(0);
    }
    else
    {
        close(pd1[0]);
        close(pd1[1]);

        close(pd3[0]);
        close(pd3[1]);

        close(pd2[1]);

        char line[N];

        do
        {
            read(pd2[0], line, N);

            if(line[strlen(line) - 1] != '.')
            {
                line[strlen(line) + 1] = '\0';
                line[strlen(line)] = '.';
            }
            printf("proc3 -> %s\n", line);
        }while(strcmp(line, "KRAJ.") != 0);

        close(pd2[0]);
        exit(0);
    }
}




















