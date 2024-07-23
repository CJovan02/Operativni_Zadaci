/*Program koji izvrsavaju procesi dete, kao argumenti se prosledjuje fajl u koji treba da se upise izlaz
  i koji tip poruke program treba da primi*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>

#define KEY 10101
#define MAX_PORUKA 255

struct poruka
{
    long tip;
    char tekst[MAX_PORUKA];
};

void main(int argc, char* argv[])
{
    int redid, tip;
    char imeFajla[50];
    struct poruka bafer;
    FILE *file;

    tip = atoi(argv[1]);
    strcpy(imeFajla, argv[2]);

    redid = msgget(KEY, 0666 | IPC_CREAT);
    if(redid < 0)
    {
        printf("Greska pri kreiranju reda poruka");
        exit(1);
    }

    int i;
    file = fopen(imeFajla, "w"); 
    do
    {
        if(msgrcv(redid, &bafer, MAX_PORUKA, tip, 0) < 0)
        {
            printf("Greska prilikom prijema poruke");
            break;
        }

        if(strcmp(bafer.tekst, "ENDOFFILE") != 0)
        {
            if(tip == 1)
                for(i = 0; i < strlen(bafer.tekst); i++)
                    bafer.tekst[i] = toupper(bafer.tekst[i]);
            else if(tip == 2)
                for(i = 0; i < strlen(bafer.tekst); i++)
                    bafer.tekst[i] = tolower(bafer.tekst[i]);

            fprintf(file, "%s", bafer.tekst);
        }
    } while (strcmp(bafer.tekst, "ENDOFFILE") != 0);

    msgctl(redid, IPC_RMID, NULL);
    fclose(file);
}