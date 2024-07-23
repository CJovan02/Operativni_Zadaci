/*Kreirati program koji moze da se prkine samo ako se kombinacija astera Ctrl-C pritisne
pet puta za redom. Ukoliko korisnik pritisne kombinaciju tastera Ctrl-Z dva puat zaredom
program na ekranu ispisuje koliko puta do sada je pritisnuto Ctrl-C*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int count, count2 = 0;

void handleInt()
{
    signal(SIGINT, handleInt);
    count++;
    if(count >= 4)
        signal(SIGINT, SIG_DFL);

    count2 = 0;
}

void handleTstp()
{
    count2++;
    
    if(count2 == 2)
    {
        printf("CTRL-C pritisnuto %d puta\n", count);
        count2 = 0;
    }
}

void main()
{
    count = 0;
    count2 = 0;

    signal(SIGINT, handleInt);
    signal(SIGTSTP, handleTstp);
    while(1)
        pause();
}