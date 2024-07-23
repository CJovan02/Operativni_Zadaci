/*
  Napisati program koji uporedjuje sadrzaj dva direktorijuma. Putanje do dir koja se uporedjuju se zadaju
  kao ulazni argumenti programa. Program za svaku regularnu datoteku iz prvog dir proverava da li postoji
  odgovarajuca datoteka u drugom dir (uporedjuje ime i velicinu datoteke). Ukolimo dat postoji stampa se
  ime dat. Napomena: Pretpostaviti da prvi dir na sadrzi poddirektorijume.
  */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

void uporedi(char* folderName, char* fileName, double size)
{
    DIR* dp;
    struct dirent* dirp;
    struct stat statBuff;
    char path[1000];

    if((dp = opendir(folderName)) == NULL)
    {
        printf("Greska pri otvaranju direktorijuma");
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

        if(S_ISREG(statBuff.st_mode) && statBuff.st_size == size && strcmp(dirp->d_name, fileName) == 0)
        {
            printf("Pronadjen fajl u drugom dir: %s, %.2fKB\n", fileName, size/1024);
        }

        if(S_ISDIR(statBuff.st_mode))
            uporedi(path, fileName, size);
    }
}

void main()
{
    DIR* dp;
    struct dirent* dirp;
    struct stat statBuff;
    char path[1000];

    char folderName[1000];
    strcpy(folderName, "prviDir");

    if((dp = opendir(folderName)) == NULL)
    {
        printf("Greska pri otvaranju prvog direktorijuma");
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

        if(S_ISREG(statBuff.st_mode))
            uporedi("drugiDir", dirp->d_name, statBuff.st_size);
    }
}