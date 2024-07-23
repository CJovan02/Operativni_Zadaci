/*Koriscenjem programskog jezika C napisati UNIX/Linux program koji svom procesu detetu,
koriscenjem redova poruka, prosleduje ulaz koji prima preko tastature, a process dete dobijene
poruke stampa na ekranu. Predvideti da se unosom teksta "QUIT" prekida rad programa.*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <string.h>

#define KEY 10101
#define MSGLEN 50

struct msgbuf 
{
    long mtype;
    char mtext[MSGLEN];
};

void main()
{
    int pid, msqid;
    struct msgbuf buf;
    buf.mtype = 1;
    strcpy(buf.mtext, "");

    msqid = msgget(KEY, 0666 | IPC_CREAT);

    if(pid = fork() != 0)
    {
        while(strcmp(buf.mtext, "QUIT") != 0)
        {
            printf("Ovo je process parent, unesite tekst: \n");
            scanf("%s", &buf.mtext);

            msgsnd(msqid, &buf, strlen(buf.mtext) + 1, 0);
        }

        wait(NULL);
    }
    else
    {
        while(strcmp(buf.mtext, "QUIT") != 0)
        {
            msgrcv(msqid, &buf, MSGLEN, 0, 0);
            printf("Ovo je process child, primljena poruka: %s\n", buf.mtext);
        }
    }

    msgctl(msqid, IPC_RMID, NULL);
}