/*Pristup bazi podataka obavlja se radi upisa i čitanja od strane više procesa. U jednom trenutku može
postojati više procesa koji čitaju sadržaj iz baze podataka procedurom read_database(), ali ako jedan
proces upisuje sadržaj u bazu podataka procedurom write_database(), nijednom drugom procesu nije
dozvoljen pristup bazi podataka radi upisa i čitanja. Prednost imaju procesi koji čitaju sadržaj, tako da
dok god ima procesa koji čitaju iz baze podataka, proces koji treba da upisuje podatke mora da čeka.
Korišćenjem programskog jezika C napisati Linux program koji korišćenjem procesa i poznatih IPC
mehanizama simulira prethodno opisani algoritam. (Sinhronizacion problem Čitaoci – pisci).*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

