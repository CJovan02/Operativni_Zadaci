/*Korišćenjem programskog jezika C napisati UNIX/Linux program koji u zadatom direktorijumu
lista nazive svih datoteka koje u svom nazivu sadrže zadati string. Putanja do direktorijuma i string
koj se traži zadaju se kao argument komandne linije.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char* argv[])
{
    DIR* dp;
    struct dirent* dirp;
    if(argc != 3)
    {
        printf("Unesi lepo argumente majmune");
        return -1;
    }

    if((dp = opendir(argv[1])) == NULL)
    {
        printf("Greska pri otvaranju direktorijuma %s", argv[1]);
        return -1;
    }

    while((dirp = readdir(dp)) != NULL)
    {
        if(strstr(dirp->d_name, argv[2]) != NULL)
            printf("%s\n", dirp->d_name);
    }
}