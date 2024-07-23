/*
  Koriscenjem programskog jezika C napisati Linux program koji kao ulazne argumente prihvata
  apsolutnu putanju do nekog direkorijuma i parametre n i m. Program ima zadatak da u zadatom
  direktorijumu iz n prvih regularnih datoteka odstampa m linija sa pocetka datoteke (pretpostaviti)
  da je maksimalna duzina linije u datoteci 100 karaktera)*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>

#define LENGTH 100

void processreg(char* fileName, int m)
{
    FILE* file = fopen(fileName, "r");

    char linija[LENGTH];
    int i;
    for(i = 0; i < m; i++)
    {
        fgets(linija, LENGTH, file);
        printf("%s", linija);
    }
    printf("\n");

    fclose(file);
}

int main(int argc, char* argv[])
{
    if(argc != 4)
    {
        printf("Unesi lepo argumente dzivljo");
        return -1;
    }

    DIR* dp;
    struct dirent* dirp;
    struct stat statBuff;
    char linija[LENGTH];

    dp = opendir(argv[1]);
    if(dp < 0)
    {
        printf("Greska pri otvaranju datoteke");
        return -1;
    }

    char path[255];
    int count = atoi(argv[2]);
    int i;
    for(i = 0; i <= count; i++)
    {
        dirp = readdir(dp);

        if(strcmp(dirp->d_name, ".") == 0
        || strcmp(dirp->d_name, "..") == 0)
            continue;

        strcpy(path, argv[1]);
        strcat(path, "/");
        strcat(path, dirp->d_name);

        stat(path, &statBuff);

        if(S_ISREG(statBuff.st_mode))
        {
            printf("Citam iz fajla %s\n", dirp->d_name);

            int brLinija = atoi(argv[3]);
            processreg(path, brLinija);
        }
    }
}