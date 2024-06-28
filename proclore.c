#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "proclore.h"

void proclore_function(int pid)
{
    // Print PID
    printf("pid: %d\n", pid);
    int pgid = getpgid(pid);
     if (pgid < 0) {
        perror("getpgid");
        return;
    }
    char status;
    char statPath[50];
    sprintf(statPath, "/proc/%d/stat", pid);
   
    FILE *statFile = fopen(statPath, "r");
    if (statFile != NULL)
    {
        fscanf(statFile, "%*d %*s %c", &status);
        fclose(statFile);

        // if(status=='w')
        //     status = 'Z';
        printf("process Status: %c", status);
        if(pid==pgid)
        {
            printf("+");
        }
        printf("\n");
    }
    else
    {
        perror("Error opening stat file");
    }
    printf("Process Group: %d\n", pgid);
    char vmPath[50];
    sprintf(vmPath, "/proc/%d/status", pid);
    FILE *vmFile = fopen(vmPath, "r");
    if (vmFile != NULL)
    {
        char line[256];
        while (fgets(line, sizeof(line), vmFile))
        {
            if (strstr(line, "VmSize:") != NULL)
            {
                printf("Virtual Memory: %s", line);
                break;
            }
        }
        fclose(vmFile);
    }
    else
    {
        perror("Error opening status file");
    }

    // Get and print Executable Path
    char exePath[256];
    sprintf(exePath, "/proc/%d/exe", pid);
    char resolvedPath[1024];
    ssize_t pathSize = readlink(exePath, resolvedPath, sizeof(resolvedPath) - 1);
    if (pathSize != -1)
    {
        resolvedPath[pathSize] = '\0';
        printf("executable Path: %s\n", resolvedPath);
    }
    else
    {
        perror("Error reading executable path");
    }
}

void proclore_function2(char *input)
{
    // printf("%s\n", input);
    char *temp = (char *)malloc(sizeof(char) * (1024));
    strcpy(temp, input);
    char *token = strtok(temp, " ");
    int c = 0;
    while (token != NULL)
    {
        c++;
        token = strtok(NULL, " ");
    }
    if (c == 1)
    {
        proclore_function(getpid());
    }
    else
    {
        strcpy(temp, input);
        char *number = (char *)malloc(sizeof(char) * (1024));
        char *token = strtok(temp, " ");
        while (token != NULL)
        {
            strcpy(number, token);
            token = strtok(NULL, " ");
        }
        // printf("%s\n", number);
        int pidd = atoi(number);
        proclore_function(pidd);
    }
    return;
}


// int pid = atoi(argv[1]);
//     int pgid = getpgid(pid);

//     if (pgid < 0) {
//         perror("getpgid");
//         return 1;
//     }

//     if (pgid == pid) {
//         printf("The process with PID %d is a foreground process.\n", pid);
//     } else {
//         printf("The process with PID %d is a background process.\n", pid);
//     }

//     return 0;
// }




