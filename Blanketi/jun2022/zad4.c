/*
  Napisati programkoji u zadatom direkorijumu rekurzivno (maksimalno do dubine 3) brise sve regularne 
  daoteke koje u imenu sadrze podstring "log" i vece su od 15KB. Za brisanje datoteka koristiti rm komandu 
  Linux operaovnog sistema.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>

#define KB 1024

void proccessdir_recursive(char* folderName, char* string, double size, int level)
{
    DIR* dp;
    struct dirent* dirp;
    struct stat statBuff;
    char path[1000];

    if((dp = opendir(folderName)) == NULL)
    {
        printf("Greska pri otvaranju foldera %s\n", folderName);
        exit(-1);
    }

    while((dirp = readdir(dp)) != NULL)
    {
        if(strcmp(dirp->d_name, ".") == 0
        || strcmp(dirp->d_name, "..") == 0)
            continue;

        strcpy(path, folderName);
        strcat(path, "/");
        strcat(path, dirp->d_name);

        //printf("%s\n", path);

        stat(path, &statBuff);

        if(S_ISREG(statBuff.st_mode) && strstr(dirp->d_name, string) != NULL && statBuff.st_size > size)
        {
            printf("Brisem fajl sa putanjom: %s\n", path);
            if(fork() == 0)
            {
                if(execlp("rm", "rm", path, NULL) < 0)
                {
                    printf("Greska prilikom pokratanja komande za brisanje");
                    exit(-1);
                }
            }
            else
                wait(NULL);
        }

        if(S_ISDIR(statBuff.st_mode) && level < 3)
            proccessdir_recursive(path, string, size, level + 1);
    }
}

void proccessdir()
{
    proccessdir_recursive("test (copy)", "log", 15*KB, 1);
}


int main()
{
    proccessdir();
}