/*Koriscenjem programskog jezika C napisati Linux/UNIX program koji ucitava podatke iz tekstualne datoteke
  poruka.txt (red po red) i zatim koriscenjem jednog reda poruka sve parne redove salje procesu koji konvertuje
  sva slova u velika, a sve neparne redove salje procesu koji konvertuje sva slova u mala. Drugi i treci proces
  konvertovane linije upisuje u datoteku sredjeno.txt.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>

#define KEY 10101
#define LENGTH 255

struct poruka{
    long tip;
    char tekst[LENGTH];
};

void main()
{
    int redid;

    FILE* izlaz = fopen("sredjeno.txt", "w");

    redid = msgget(KEY, 0666 | IPC_CREAT);
    if(redid < 0)
    {
        printf("Greska pri kreiranju reda poruka");
        exit(-1);
    }

    if(fork() != 0)
    {
        struct poruka buffer;

        FILE* file = fopen("poruka.txt", "r");

        int count = 1;
        while(!feof(file))
        {
            fgets(buffer.tekst, LENGTH, file);
            if(count % 2 == 0)
                buffer.tip = 1;
            else
                buffer.tip = 2;
            
            if(msgsnd(redid, &buffer, sizeof(char) * LENGTH, 0) < 0)
            {
                printf("Greska pri slanju poruke");
                exit(-1);
            }
            count++;
        }

        strcpy(buffer.tekst, "ENDOFFILE");
        buffer.tip = 1;
        msgsnd(redid, &buffer, sizeof(char) * LENGTH, 0);
        buffer.tip = 2;
        msgsnd(redid, &buffer, sizeof(char) * LENGTH, 0);

        fclose(file);
        wait(NULL);
        fclose(izlaz);
        msgctl(redid, IPC_RMID, NULL);
        exit(0);
    }
    else if(fork() != 0)
    {
        struct poruka buffer;
        int i;

        while(strcmp(buffer.tekst, "ENDOFFILE") != 0)
        {
            if(msgrcv(redid, &buffer, sizeof(char) * LENGTH, 1, 0) < 0)
            {
                printf("Greska pri prijemu poruke");
                exit(-1);
            }

            if(strcmp(buffer.tekst, "ENDOFFILE") != 0)
            {
                for(i = 0; i < strlen(buffer.tekst); i++)
                    buffer.tekst[i] = toupper(buffer.tekst[i]);
                fprintf(izlaz, "%s", buffer.tekst);
            }
        }

        wait(NULL);
    }
    else
    {
        struct poruka buffer;

        while(strcmp(buffer.tekst, "ENDOFFILE") != 0)
        {
            if(msgrcv(redid, &buffer, sizeof(char) * LENGTH, 2, 0) < 0)
            {
                printf("Greska pri prijemu poruke");
                exit(-1);
            }

            int i;
            if(strcmp(buffer.tekst, "ENDOFFILE") != 0)
            {
                for(i = 0; i < strlen(buffer.tekst); i++)
                    buffer.tekst[i] = tolower(buffer.tekst[i]);
                fprintf(izlaz, "%s", buffer.tekst);
            }
        }
    }
}