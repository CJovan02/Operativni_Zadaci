/*Korišćenjem programskog jezika C napisati Linux/UNIX program koji u zadatom direktorijumu
(naziv direktorijuma se zadaje kao prvi argument komandne linije) pronalazi i štampa nazive svih
datoteka koje u imenu sadrže string koji se zadaje kao drugi argument komandne linije.
Pretraživanje se obavlja rekurzivno u zadatom direktorijumu i svim njegovim poddirektorijumima.*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

void processdir(char* folderName, char* str)
{
    DIR* dp;
    struct stat statBuff;
    struct dirent* dirp;
    int result;
    char path[80];

    if((dp = opendir(folderName)) == NULL)
    {
        printf("Greska pri otvaranju direktorijuma %s", folderName);
        exit(-1);
    }

    while((dirp = readdir(dp)) != NULL)
    {
        strcpy(path, folderName);
        strcat(path, "/");
        strcat(path, dirp->d_name);

        printf("Obradjujem sada: %s\n", path);

        if((result = stat(path, &statBuff)) == -1)
        {
            printf("Greska pri citanju podataka o objektu");
            continue;
        }

        if((strstr(dirp->d_name, str)) != NULL)
            printf("Pronasao: %s\n", dirp->d_name);

        if(S_ISDIR(statBuff.st_mode)
        && strcmp(dirp->d_name, ".") != 0
        && strcmp(dirp->d_name, "..") != 0)
            processdir(path, str);
    }
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Unesi lepo argumente programa majmune");
        return -1;
    }

    processdir(argv[1], argv[2]);
}