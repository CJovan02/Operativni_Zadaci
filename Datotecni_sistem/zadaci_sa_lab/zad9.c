/*Koriscenjem programskog jezika C napisati Linux program koji u zadatom direktorijumu
 (putanja do direktorijuma se zadaje kao ulazni argument programa) rekurizvno nalazi pet najvecih regularnih
  datoteka. Program na kraju rada treba da odstampa putanje i velicine tih datoteka.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

#define N 5

void prikazi(char putanje[][255], float velicine[])
{
    int i;
    for(i = 0; i < N; i++)
        printf("%s\t%.2fKB\n", putanje[i], velicine[i] / 1024);
}

void sortiraj(char putanje[][255], float velicine[])
{
    int i, j;
    long tmpVelicine;
    char tmpPutanje[255];

    for(i = 0; i < N - 1; i++)
    {
        for(j = i + 1; j < N; j++)
        {
            if(velicine[i] > velicine[j])
            {
                strcpy(tmpPutanje, putanje[i]);
                strcpy(putanje[i], putanje[j]);
                strcpy(putanje[j], tmpPutanje);

                tmpVelicine = velicine[i];
                velicine[i] = velicine[j];
                velicine[j] = tmpVelicine;
            }
        }
    }
}

void processdir(char* folderName, char putanje[][255], float velicine[])
{
    DIR* dp;
    struct dirent* dirp;
    struct stat statBuff;

    if((dp = opendir(folderName)) == NULL)
    {
        printf("Greska pri otvaranju foldera");
        exit(-1);
    }

    static int k = 0;
    char path[255];
    while((dirp = readdir(dp)) != NULL)
    {
        if (strcmp(dirp->d_name, ".") == 0
         || strcmp(dirp->d_name, "..") == 0)
            continue;

        strcpy(path, folderName);
        strcat(path, "/");
        strcat(path, dirp->d_name);

        stat(path, &statBuff);

        if(S_ISREG(statBuff.st_mode))
        {
            if(k < N)
            {
                velicine[k] = statBuff.st_size;
                strcpy(putanje[k++], path);
            }
            else
            {
                sortiraj(putanje, velicine);
                if(velicine[0] < statBuff.st_size)
                {
                    strcpy(putanje[0], path);
                    velicine[0] = statBuff.st_size;
                }
            }
        }

        if(S_ISDIR(statBuff.st_mode))
            processdir(path, putanje, velicine);
    }
}

int main(int argc, char* argv[])
{
    // if(argc != 2)
    // {
    //     printf("Unesi lepo argumente vole");
    //     return -1;
    // }

    char putanje[N][255];
    float velicine[N];

    processdir("test", putanje, velicine);
    sortiraj(putanje, velicine);
    prikazi(putanje, velicine);
}