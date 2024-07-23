/*Korišćenjem programskog jezika C napisati Linux program koji za tekući direktorijum generiše
statistiku tako što prebrojava: podfoldere, linkove, prazne regularne datoteke i regularne datoteke čija
je veličina veća od 0B. Na kraju program na ekranu štampa ovu statistiku (brojeve stavki po tipu).*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

int main(int argc, char* argv[])
{
    int countSubfolder = 0, countLink = 0, countEmptyFile = 0, countFile = 0;
    DIR* dp;
    struct dirent* dirp;
    struct stat statBuff;
    char path[1024];
    char folderName[1024];

    //uzima putanju do trenutne datoteke
    getcwd(folderName, sizeof(folderName));

    if((dp = opendir(folderName)) == NULL)
    {
        printf("Greska pri otvaranju trenutne datoteke");
        return -1;
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

        if(S_ISDIR(statBuff.st_mode))
            countSubfolder++;
        if(S_ISLNK(statBuff.st_mode))
            countLink++;
        if(S_ISREG(statBuff.st_mode) && statBuff.st_size > 0)
            countFile++;
        else if(statBuff.st_size == 0)
            countEmptyFile++;
    }

    printf("Statistika za trenutnu datoteku:\nPodfolderi: %d\nLinkovi: %d\nPrazni fajlovi: %d\nFajlovi: %d\n", 
            countSubfolder, countLink, countEmptyFile, countFile);
}