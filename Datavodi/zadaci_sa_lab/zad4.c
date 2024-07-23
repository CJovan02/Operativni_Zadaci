/*
 Kreirati dva Linux procesa koja komuniciraju koriscenjem datavoda. Glavni proces iz izvorisne datoteke cita 100
 karatktera i korisnjecem datavoda procitane karaktere prosledjuje durgom procesu koji ih upisuje u odredisnu
 datoteku cije se ime prosledjuje kao drugi argument komandne linije. Ova procedure se ponavlja dok se ne iskopira
 kompletna datoteka.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int pd[2], pid;
    FILE *izvor, *odrediste;

//     if(argc != 3)
//     {
//         printf("Unesite ime fajla za citanje i za upis!");
//         return -1;
//     }

    if(pipe(pd) == -1)
    {
        printf("Doslo je do greske pri kreiranju datavoda");
        return -1;
    }

    if(pid = fork() == 0)
    {
        char data[101];
        int uslov = 1;
        close(pd[0]);

        izvor = fopen("zad4Ulaz.txt", "r");
        //izvor = fopen(argv[1], "r");

        while(!feof(izvor))
        {
            write(pd[1], &uslov, sizeof(int));
            fgets(data, 100, izvor);
            write(pd[1], data, strlen(data) + 1);
            //printf("%s", data);
        }

        uslov = 0;
        write(pd[1], &uslov, sizeof(int));

        fclose(izvor);
        close(pd[1]);
        exit(0);
    }
    else
    {
        close(pd[1]);

        char data[101];
        int uslov;
        odrediste = fopen("zad4Izlaz.txt", "w");
//         odrediste = fopen(argv[2], "w");

        do
        {
            read(pd[0], &uslov, sizeof(int));
            if(uslov == 1)
            {
                read(pd[0], data, 100);
                //printf("%s", data);
                fprintf(odrediste, "%s", data);
            }
        }while(uslov == 1);

        fclose(odrediste);
        wait(NULL);
        close(pd[0]);
        return 0;
    }
}








