/*Korišćenjem programskog jezika C napisati UNIX/Linux program koji može da se prekine samo
ako se kombinacija tastera Ctrl-C pritisne pet puta za redom. Ukoliko korisnik pritisne kombinaciju
tastera Ctrl-Z program na ekranu ispisuje koliko puta do sada je pritisnuto Ctrl-C*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define threshold 5
int brojac;

void ctrlC(int sig_num)
{
    signal(SIGINT,  ctrlC);
    brojac++;

    if(brojac >= threshold - 1)
        signal(SIGINT, SIG_DFL);
    else
    {
        printf(" Continuing..\n");
        fflush(stdout);
    }
}

void ctrlZ(int sig_num)
{
    signal(SIGTSTP, ctrlZ);
    printf("Ctrl+C prisnuto  %d puta\n", brojac);
    fflush(stdout);
}

void main()
{
    brojac = 0;

    signal(SIGINT, ctrlC);
    signal(SIGTSTP, ctrlZ);
    while(1)
        pause();
}