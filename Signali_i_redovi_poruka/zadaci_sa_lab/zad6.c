/*Korišćenjem programskog jezika C napisati Linux/UNIX program koji kreira dodatni process sa kojim
  komunicira dvosmerno korišćenjem jednog reda poruka. Proces roditelj iz datoteke čije je ime prosleđeno
  kao argument komandne linije čita liniju po liniju teksta i svaku liniju šalje procesu detetu u posebnoj poruci. 
  Proces dete u svakoj primljenoj liniji prebrojava broj pojavljivanja velikih slova abecede i procesu roditelju 
  vraća kroz isti red poruka za svaku analiziranu liniju teksta kroz posebnu poruku redni broj linije i broj
  pojavljivanja velikih slova. Glavni program te podatke štampa na ekranu (redni broj linije i broj pojavljivanja
  velikih slova). Kada se analiziraju sve linije teksta iz datoteke glavni program na ekranu treba da odštampa 
  "KRAJ".*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <ctype.h>

#define KEY 10101
#define LENGTH 255

struct poruka
{
    long tip;
    char tekst[LENGTH];
};

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Niste lepo uneli argumente programa");
        exit(1);
    }

    int pid, redid;
    FILE* file;

    redid = msgget(KEY, 0666 | IPC_CREAT);
    if(redid < 0)
    {
        printf("Greska pri kreiranju reda poruka");
        exit(1);
    }

    if(pid = fork() != 0)
    {
        struct poruka bafer;
        struct poruka bafer2;
        file = fopen(argv[1], "r");

        while(!feof(file))
        {
            bafer.tip = 1;
            fgets(bafer.tekst, LENGTH, file);
            //printf("Roditelj salje: %s\n", bafer.tekst);
            if(msgsnd(redid, &bafer, LENGTH, 0) < 0)
            {
                printf("Greska pri slanju poruke");
                exit(1);
            }
            
            fflush(stdout);
            msgrcv(redid, &bafer2, LENGTH, 2, 0);
            printf("Linija %s", bafer2.tekst);
            msgrcv(redid, &bafer2, LENGTH, 2, 0);
            printf(" ima %s velikih slova\n", bafer2.tekst);
        }
        strcpy(bafer.tekst, "ENDOFFILE");
        msgsnd(redid, &bafer, LENGTH, 0);

        fclose(file);
        wait(NULL);
        msgctl(redid, IPC_RMID, NULL);
    }
    else
    {
        struct poruka bafer;
        struct poruka bafer2;
        int count, brLinije = 0, i;
        do
        {
            if(msgrcv(redid, &bafer, LENGTH, 1, 0) < 0)
            {
                printf("Greska pri prijemu poruke");
                exit(1);
            }

            if(strcmp(bafer.tekst, "ENDOFFILE") != 0)
            {
                count = 0;
                for(i = 0; i < strlen(bafer.tekst); i++)
                    if(bafer.tekst[i] == toupper(bafer.tekst[i]))
                        count++;

                brLinije++;

                bafer2.tip = 2;
                sprintf(bafer2.tekst, "%d", brLinije);
                msgsnd(redid, &bafer2, LENGTH, 0);

                bafer2.tip = 2;
                sprintf(bafer2.tekst, "%d", count);
                msgsnd(redid, &bafer2, LENGTH, 0);

                //printf("Linija sa rednim brojem %d ima %d velikih slova\n", brLinije, count);
            }
            
        } while (strcmp(bafer.tekst, "ENDOFFILE") != 0);
        
    }

    return 0;
}