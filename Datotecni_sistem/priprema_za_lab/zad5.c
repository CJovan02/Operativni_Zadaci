/*Korišćenjem programskog jezika C napisati Linux/UNIX program koji u zadatom direktorijumu
rekurzivno (maksimalno do dubine 3) briše sve regularne datoteke čija je veličina veća od 10KB.
Za brisanje datoteka koristiti rm komandu Linux operativnog sistema.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define KB 1024

void processdir(char* folderName, int level)
{
    DIR* dp;
    struct stat statBuff;
    struct dirent* dirp;
    char path[KB];

    if((dp = opendir(folderName)) == NULL)
    {
        printf("Greska pri otvaranju direktorijuma");
        exit(-1);
    }

    while((dirp = readdir(dp)) != NULL)
    {
        if (strcmp(dirp->d_name, ".") == 0
         || strcmp(dirp->d_name, "..") == 0)
            continue;

        strcpy(path, folderName);
        strcat(path, "/");
        strcat(path, dirp->d_name);

        printf("%s\n", path);

        stat(path, &statBuff);

        if(S_ISREG(statBuff.st_mode) && statBuff.st_size > 10 * KB)
        {
            //nemam pojama zasto mora da se forkuje proces
            if(fork() == 0)
            {    
                printf("Brisem %s\n", path);
                if(execlp("rm", "rm", path, NULL) < 0)
                {
                    printf("Greska prilikom pozivanja komande za brisanje");
                    exit(-1);
                }
            }
            else
                wait(NULL);
        }
        else if(S_ISDIR(statBuff.st_mode) && level < 3)
            processdir(path, level + 1);
    }

    closedir(dp);
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Unesi lepo argumente svrsljo");
        return -1;
    }

    processdir(argv[1], 1);
}