/*Korišćenjem programskog jezika C napisati UNIX/Linux program koji od korisnika očekuje da sa
tastature unese korisničko ime. Ukoliko korisnik ne unese ništa u roku od 30 sekundi proces
obaveštava korisnika da je vreme za unos isteklo i izlazi.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void istekloVreme(int sig_num)
{
    printf("Vreme vam je isteklo, izlazim iz programa\n");
    exit(0);
}

void main()
{
    signal(SIGALRM, istekloVreme);

    char ime[30];

    printf("Unesite korisnicko ime: \n");
    alarm(30);
    gets(ime);
    alarm(0);

    printf("Username: %s", ime);
    exit(0);
}