/*Korišćenjem programskog jezika C napisati UNIX/Linux program koji onemogućava da se tekući
proces prekine jednim pritiskom kombinacije tastera Ctrl-C. Proces se prekida tek kada se ova
kombinacija tastera pritisne dva puta za redom*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void obrada(int sig_num)
{
    signal(SIGINT, SIG_DFL);
    printf("Morate jos jednom pritisnuti istu komandu\n");
    fflush(stdout);
}

void main()
{
    signal(SIGINT, obrada);
    while(1)
        pause();
}