/*
  Kreirati Linux program koji kopira uz kriptovanje datoteku cije se ima prosledjuje kao prvi argument komendne
  linije tako so kreira dodatni proces. Glavni proces iz izvorisne datoteke cita 200 bajtova i koriscenjem
  datavoda
  procitane bajtove prosledjuje drugom procesu koji te bajtove upisuje u odredisnu datoteku cije se ime
  prosledjuje
  kao drugi argument komandne linije ali tako da svaki bajt XOR-uje generisanom slucajnom vrednoscu od 1 do 255.
  Drugi proces tu slucajno generisanu vrednost prvo upisuje u izlaznu datoteku pa onda bajtove datoteke koja se
  kopira. Ova procedura se ponavlja dok se ne iskopira kompletna datoteka.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define N 200

int main(int argc, char* argv[])
{
    int pid, pd[2], i;
    FILE *izvor, *odrediste;
    srand(time(0));

//     if(argc != 3)
//     {
//         printf("Morate uneti ime izvora i odredista pri pozivu programa");
//         return -1;
//     }

    if(pipe(pd) == -1)
    {
        printf("Greska pri kreiranju datavoda");
        return -1;
    }

    if(pid = fork() != 0)
    {
        close(pd[0]);
        char data[N];

        izvor = fopen("izvor.txt", "r");
//         izvor = fopen(argv[1], "r");
        if(izvor == NULL)
        {
            printf("Greska pri otvaranju izvornog fajla");
            return -1;
        }

        while(!feof(izvor))
        {
            fgets(data, N, izvor);
            write(pd[1], data, N);
        }

        fclose(izvor);
        close(pd[1]);
        wait(NULL);
    }
    else
    {
        close(pd[1]);

        int count, key, pom;
        char data[N];

        odrediste = fopen("odrediste.txt", "w");
//         odrediste = fopen(argv[2], "w");
        if(odrediste == NULL)
        {
            printf("Greska pri otvaranju odredisnog fajla");
            return -1;
        }

        count = read(pd[0], data, N);
        while(count != 0)
        {
            key = (rand() % 255) + 1;
            fprintf(odrediste, "key: %d\t", key);
            for(i = 0; i < N; i++)
            {
                pom = data[i] - '0'; //konvertovanje u int
                pom = pom ^ key; //XOR-uje se bajt
                data[i] = pom + '0'; //upisuje se sifrovani baj
            }
            fprintf(odrediste, "%s\n\n\n", data);

            count = read(pd[0], data, N);
        }

        fclose(odrediste);
        close(pd[0]);
        exit(0);
    }
}






















