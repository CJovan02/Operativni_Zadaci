/*KoriS¢enjem programskog jezika C napisati Linux program koji u zadatom direktorijumu (kao ulazni
argument programa zadaje se apsolutna putanja do direktorijuma) i njegovim poddirektorijumima (maksimalno
do nivoa 5) odreduje i Stampa naziv najvece regularne datoteke.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

void processdir_recursive(char* folderName, char* maxReg, float *maxSize, int level)
{
    DIR* dp;
    struct dirent* dirp;
    struct stat statBuff;

    if((dp = opendir(folderName)) == NULL)
    {
        printf("Nije moguce otvoriti prosledjeni folder");
        exit(-1);
    }

    char path[255];
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
        {
            if(statBuff.st_size > *maxSize)
            {
                *maxSize = statBuff.st_size;
                strcpy(maxReg, dirp->d_name);
            }
        }

        if(S_ISDIR(statBuff.st_mode) && level < 5)
            processdir_recursive(path, maxReg, maxSize, level + 1);
    }

    closedir(dp);
}

void processdir(char* folderName, char* maxReg, float *maxSize)
{
    *maxSize = 0;
    processdir_recursive(folderName, maxReg, maxSize, 1);
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Unesi lepo argumente rkljo\n");
        return -1;
    }

    char maxReg[255];
    float maxSize;

    processdir(argv[1], maxReg, &maxSize);

    printf("%s\t%.2fKB\n", maxReg, maxSize / 1024);
}