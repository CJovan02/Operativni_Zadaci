/*
  Korišćenjem programskog jezika C kreirati Linux program pronadjiDatoteku koji radi rekurzivno pretraživanje
  stabla zadatog direktorijuma u potrazi za zadatom datotekom i štampa njenu lokaciju i veličinu. Argumenti
  komandne linije programa su direktorijum koji se pretražuje i naziv datoteke koja se traži. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

void pronadjiDatoteku(char* folderName, char* fileName)
{
    DIR* dp;
    struct dirent* dirp;
    struct stat statBuff;
    char path[255];

    if((dp = opendir(folderName)) == NULL)
    {
        printf("Greska pri otvaranju direktorijuma %s", folderName);
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

        stat(path, &statBuff);

        //printf("%s\n", path);

        if(S_ISREG(statBuff.st_mode) && strcmp(dirp->d_name, fileName) == 0)
        {
            printf("Pronadjena datoteka %s\nPutanja: %s\nVelicina: %.2fKB\n", fileName, path, (float)statBuff.st_size / 1024);
            return;
        }

        if(S_ISDIR(statBuff.st_mode))
            pronadjiDatoteku(path, fileName);
    }

    printf("Nije pronadjena datoteka koju trazite :(\n");
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Unesi lepo argumente krsljo\n");
        return -1;
    }

    pronadjiDatoteku(argv[1], argv[2]);
}