/*Korišćenjem programskog jezika C napisati UNIX/Linux program koji prikazuje statistiku za zadati
direktorijum (broj poddirektorijuma, regularnih datoteka i linkova). Putanja do direktorijuma
(apsolutna ili relativna) se zadaje kao argument komandne linije.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
    DIR* dp;
    struct dirent* dirp;
    char path[80];
    int countDir = 0, countReg = 0, countLink = 0;
    struct stat statbuff;
    int result;

    if(argc != 2)
    {
        printf("Unesi lepo argumente majmune");
        return -1;
    }

    if((dp = opendir(argv[1])) == NULL)
    {
        printf("Greska pri otvaranju dir: %s", argv[1]);
        return -1;
    }

    while((dirp = readdir(dp)) != NULL)
    {
        strcpy(path, argv[1]);
        strcat(path, "/");
        strcat(path, dirp->d_name);

        printf("Otvaram fajl sa putanjom: %s\n", path);

        if((result = stat(path, &statbuff)) == -1)
        {
            printf("Nemogce procitati statistiku za ovu putanju");
            continue;
        }

        if(S_ISDIR(statbuff.st_mode))
            countDir++;
        if(S_ISREG(statbuff.st_mode))
            countReg++;
        if(S_ISLNK(statbuff.st_mode))
            countLink++;
    }

    printf("Zadat folder ima %d poddirektorijuma, %d datoteka i %d linkova", countDir, countReg, countLink);
}