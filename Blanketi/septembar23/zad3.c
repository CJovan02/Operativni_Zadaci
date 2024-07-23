/*Koriscenjem programskog jezika C napisati Linux/UNIX program koji ucitava podatke iz tekstualne datoteke poruka.txt (red po red) i zatim koriscenjem jednog reda poruka sve parne redove salje procesu koji konvertuje sva slova u velika, a sve neparne redove salje procesu koji konvertuje sva slova u mala. Drugi i treci proces konvertovane linije upisuje u datoteku sredjeno.txt.*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>

struct msgbuf{
    long mtype;
    char mtext[255];
};

int main(){
    int msqid,pid1,pid2,brLinije,i;
    char tmp[255];
    FILE *ulazniFajl,*izlazniFajl;
    struct msgbuf buf1,buf2,buf3;

    if((msqid=msgget(10001,IPC_CREAT | 0666))==-1){
        printf("Greska pri kreiranju reda poruka!\n");
        return -1;
    }
    ulazniFajl=fopen("poruka.txt","r");
    if(ulazniFajl==NULL){
        printf("Greska pri otvaranju fajla!\n");
        return -1;
    }
    izlazniFajl=fopen("sredjeno.txt","w");
    if(izlazniFajl==NULL){
        printf("Greska pri kreiranju fajla!\n");
        return -1;
    }

    brLinije=1;
    if((pid1=fork())!=0){
        while(1){
            //printf("A\n");
            if(fgets(buf1.mtext,255,ulazniFajl)==NULL){
                buf1.mtype=2;
                strcpy(buf1.mtext,"KRAJ");
                if((msgsnd(msqid,&buf1,255,0))==-1){
                    printf("Greska pri slanju poruke!\n");
                    return -1;
                }
                buf1.mtype=3;
                if((msgsnd(msqid,&buf1,255,0))==-1){
                    printf("Greska pri slanju poruke!\n");
                    return -1;
                }
                printf("Poslata poruka za kraj!\n");
                break;
            }
            else if(brLinije%2!=0){
                //puts(buf1.mtext);
                brLinije++;
                buf1.mtype=2;
                if((msgsnd(msqid,&buf1,255,0))==-1){
                    printf("Greska pri slanju poruke!\n");
                    return -1;
                }
            }
            else{
                //puts(buf1.mtext);
                brLinije++;
                buf1.mtype=3;
                if((msgsnd(msqid,&buf1,255,0))==-1){
                    printf("Greska pri slanju poruke!\n");
                    return -1;
                }
            }
        }
        wait(NULL);
        fclose(ulazniFajl);
        fclose(izlazniFajl);
        msgctl(msqid,IPC_RMID,0);
        return 0;
    }
    else if((pid2=fork())!=0){
        while(1){
            if((msgrcv(msqid,&buf2,255,2,0))==-1){
                printf("Greska pri prijemu poruke!\n");
                return -1;
            }
            puts(buf2.mtext);
            if(strcmp(buf2.mtext,"KRAJ")==0){
                break;
            }
            for(i=0;i<strlen(buf2.mtext);i++){
                if(buf2.mtext[i]>='a' && buf2.mtext[i]<='z'){
                    buf2.mtext[i]=toupper(buf2.mtext[i]);
                }
            }
            puts(buf2.mtext);
        }
        wait(NULL);
    }
    else{
        while(1){
            if((msgrcv(msqid,&buf3,255,3,0))==-1){
                printf("Greska pri prijemu poruke!\n");
                return -1;
            }
            puts(buf3.mtext);
            if(strcmp(buf3.mtext,"KRAJ")==0){
                break;
            }
            for(i=0;i<strlen(buf3.mtext);i++){
                if(buf3.mtext[i]>='A' && buf3.mtext[i]<='Z'){
                   buf3.mtext[i]=tolower(buf3.mtext[i]);
                }
            }
            puts(buf3.mtext);
        }
        
    }
}