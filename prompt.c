#include "headers.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#define clear() printf("\033[H\033[J")

void prompt(char *Pardir)
{
    char *Username = getenv("USER");
    printf("\033[1;32m%s\033[0m", Username);
    printf("\033[1;32m@\033[0m");
    char *Hostname = (char *)malloc(sizeof(char) * (1024));
    gethostname(Hostname, 1024);
    printf("\033[1;32m%s\033[0m", Hostname);
    printf(":");
    char *CurrentDir = (char *)malloc(sizeof(char) * (1024));
    getcwd(CurrentDir, 1024);
    if (strlen(CurrentDir) < strlen(Pardir))
    {
        printf("\033[1;34m%s\033[0m ", CurrentDir);
        if (ttiimmee > 2)
        {
            printf("<%s : %ds>", command, ttiimmee);
        }
        printf("$ ");
        ttiimmee = 0;
    }
    else
    {
        printf("\033[1;34m~/\033[0m");
        for (int i = strlen(Pardir); i < strlen(CurrentDir); i++)
        {
            printf("\033[1;34m%c\033[0m", CurrentDir[i]);
        }
        if (ttiimmee > 2)
        {
            printf("<%s : %ds>", command, ttiimmee);
        }
        printf("$ ");
        ttiimmee = 0;
    }
    printf("%s", "\033[1m\033[0m");
    return;
}