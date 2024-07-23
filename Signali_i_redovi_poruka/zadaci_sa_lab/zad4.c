/*
  Korisnik unosi korisnicko ime i lozinku, ako nakon unosa korisnickog imena ne unese sifru u roku od
  60 sekundi obavestava se i program se prekida*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void alarmHandler(int sigNum)
{
    printf("Vreme za unos sifre je isteklo, program se gasi...\n");
    exit(0);
}

void main()
{
    signal(SIGALRM, alarmHandler);
    printf("Unesite korisnicko ime: ");
    gets();
    
    alarm(60);
    printf("Unesite lozinku: ");
    gets();

    exit(0);
}