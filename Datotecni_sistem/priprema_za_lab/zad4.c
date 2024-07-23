/*Korišćenjem programskog jezika C napisati Linux/UNIX program koji predstavlja verziju ls
komande. Program kao argument ima ime direktorijuma, a prikazuje listu direktorijuma u njemu,
liniju po liniju, sa sledećim podacima: ime direktorijuma, broj datoteka koje se u njemu nalaze,
veličinu u bajtovima (veličina predstavlja zbir veličina datoteka koje se nalaze u direktorijumu)*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

void processdir(char* folderName, int* count, double* size)
{
    DIR* dp;
    struct dirent* dirp;
    struct stat statBuff;

    if((dp = opendir(folderName)) == NULL)
    {
        printf("Greska u otvaranju foldera");
        exit(-1);
    }

    char path[1024];
    while((dirp = readdir(dp)) != NULL)
    {
        strcpy(path, folderName);
        strcat(path, "/");
        strcat(path, dirp->d_name);

        stat(path, &statBuff);

        if(S_ISREG(statBuff.st_mode))
        {
            *count++;
            *size += statBuff.st_size;
        }
    }

    closedir(dp);
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Unesi lepo argumente cisljo");
        return -1;
    }

    DIR* dp;
    struct dirent* dirp;
    struct stat statBuff;

    if((dp = opendir(argv[1])) == NULL)
    {
        printf("Greska pri otvaranju fajla");
        return -1;
    }

    char path[1024];
    while((dirp = readdir(dp)) != NULL)
    {
        if (strcmp(dirp->d_name, ".") == 0
         || strcmp(dirp->d_name, "..") == 0)
            continue;

        strcpy(path, argv[1]);
        strcat(path, "/");
        strcat(path, dirp->d_name);

        //printf("%s\n", path);

        stat(path, &statBuff);

        int count;
        double size;
        if(S_ISDIR(statBuff.st_mode))
        {
            processdir(path, &count, &size);
            printf("%s\t%d\t%.2fKB\n", dirp->d_name, count, size/1024);    
        }
    }

    closedir(dp);
}