/*Korišćenjem programskog jezika C napisati Linux program koji u zadatom direktorijumu 
(kao ulazni argument programa zadaje se apsolutna putanja do direktorijuma) i njegovim poddirektorijumima 
određuje i štampa imena svih regularnih datoteka koje su veće od 100KB tako da je dobijeni spisak sortiran po 
veličini datoteke u rastućem redosledu.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

#define N 100
#define KB 1024

void sortirajIPrikazi(char putanje[][255], float velicine[], int length)
{
    int i, j;
    char tmpPutanja[255];
    float tmpVelicina;

    for(i = 0; i < length - 1; i++)
    {
        for(j = i + 1; j < length; j++)
        {
            if(velicine[i] > velicine[j])
            {
                tmpVelicina = velicine[i];
                velicine[i] = velicine[j];
                velicine[j] = tmpVelicina;

                strcpy(tmpPutanja, putanje[i]);
                strcpy(putanje[i], putanje[j]);
                strcpy(putanje[j], tmpPutanja);
            }
        }
    }

    for(i = 0; i < length; i++)
        printf("%s\t%.2f\n", putanje[i], velicine[i]/KB);
}

void processreg(char* path, char putanje[][255], float velicine[], int* index)
{
    struct stat statBuff;

    stat(path, &statBuff);

    if(statBuff.st_size > 20*KB)
    {
        strcpy(putanje[*index], path);
        velicine[(*index)++] = statBuff.st_size;
    }
}

void processdir(char* folderName, char putanje[][255], float velicine[], int* count)
{
    DIR* dp;
    struct dirent* dirp;
    struct stat statBuff;
    char path[255];

    if((dp = opendir(folderName)) == NULL)
    {
        printf("Greska prilikom otvaranja datoteke");
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
            processreg(path, putanje, velicine, count);
    }
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Unesi lepo argumente nilski konju jedan");
        return -1;
    }

    char putanje[N][255];
    float velicine[N];
    int count = 0;

    DIR* dp;
    struct dirent* dirp;
    struct stat statBuff;
    char path[255];

    if((dp = opendir(argv[1])) == NULL)
    {
        printf("Greska prilikom otvaranja datoteke");
        return -1;
    }

    while((dirp = readdir(dp)) != NULL)
    {
        if(strcmp(dirp->d_name, ".") == 0
        || strcmp(dirp->d_name, "..") == 0)
            continue;

        strcpy(path, argv[1]);
        strcat(path, "/");
        strcat(path, dirp->d_name);

        stat(path, &statBuff);

        if(S_ISREG(statBuff.st_mode))
            processreg(path, putanje, velicine, &count);

        if(S_ISDIR(statBuff.st_mode))
            processdir(path, putanje, velicine, &count);
    }

    //printf("%d\n", count);
    sortirajIPrikazi(putanje, velicine, count);

    return 0;
}