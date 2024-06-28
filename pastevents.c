#include "headers.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include "pastevents.h"
#include "caller.h"
#include "divider.h"
char *pastevents1 = "pastevents";

void pastevents_function(char *input, int bgg)
{
    if (bgg == 1)
    {
        printf("[%d]\n", getpid());
    }
    FILE *fptr;
    char *finalfile = (char *)malloc(sizeof(char) * 1024);
    strcpy(finalfile, Pardir);
    strcat(finalfile, "/");
    strcat(finalfile, "past.txt");
    fptr = fopen(finalfile, "r+");
    if (fptr == NULL)
    {
        perror("Error opening file for reading");
        return;
    }
    char *temp = (char *)malloc(sizeof(char) * 1024);
    strcpy(temp, input);
    char *token = strtok(temp, " ");
    int f = 0;
    if (strcmp(pastevents1, token) == 0)
        f = 1;
    int c = 0;
    char *num = (char *)malloc(sizeof(char) * 1024);
    int sigmaflag = 0;
    int sigmaflag2 = 0;
    while (token != NULL)
    {
        c++;
        strcpy(num, token);
        if (c == 2)
        {
            if (strcmp(token, "purge") == 0)
                sigmaflag = 1;
        }
        if (c == 2)
        {
            if (strcmp(token, "execute") == 0)
                sigmaflag2 = 1;
        }

        token = strtok(NULL, " ");
    }
    if (c == 1)
    {
        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), fptr) != NULL)
        {
            printf("%s", buffer);
        }
        printf("\n");
    }
    else if (c == 2)
    {

        if (sigmaflag)
        {
            if (ftruncate(fileno(fptr), 0) != 0)
            {
                perror("Error truncating file");
                fclose(fptr);
                return;
            }
        }
        else
        {
            printf("Invalid ccc\n");
            return;
        }
    }
    else
    {
        if (!sigmaflag2)
        {
            printf("Invalid ccc\n");
            return;
        }
        char buffer[1024];        // Buffer to store read data
        char lastLine[1024] = ""; // Buffer to store the last line
        int ct = 0;
        // printf("%s\n", num);
        int number = atoi(num);
        // printf("%d\n", number);
        int totallines = 0;
        while (fgets(buffer, sizeof(buffer), fptr) != NULL)
        {
            totallines++;
        }
        // printf("%d\n", totallines);
        fclose(fptr);
        fptr = fopen(finalfile, "r+");
        char buffer2[1024];
        int go = 0;
        while (fgets(buffer2, sizeof(buffer2), fptr) != NULL)
        {
            ct++;
            if (ct == totallines - number + 1)
            {
                go = 1;
                divider2(buffer2);
            }
            strcpy(lastLine, buffer); // Store the current line in lastLine
        }
        if (go == 0)
        {
            printf("the file doesnot have that much history\n");
            fclose(fptr);
            return;
        }
    }
    fclose(fptr);
    return;
}

void store(char *input)
{
    // printf("%s from\n", input);
    char *temptemp = (char *)malloc(sizeof(char) * 1024);
    strcpy(temptemp, input);
    char *toto = strtok(temptemp, " ");
    while (toto != NULL)
    {
        if(strcmp(toto,"pastevents")==0)
            return;
        toto = strtok(NULL, " ");
    }
    // printf("%s adsad\n", input);
    FILE *fptr;
    char *finalfile = (char *)malloc(sizeof(char) * 1024);
    strcpy(finalfile, Pardir);
    strcat(finalfile, "/");
    strcat(finalfile, "past.txt");
    fptr = fopen(finalfile, "r+");
    if (fptr == NULL)
    {
        perror("Error opening file for reading");
        return;
    }
    char buffer[1024];
    int totallines = 0;
    while (fgets(buffer, sizeof(buffer), fptr) != NULL)
    {
        totallines++;
    }
    char **all = (char **)malloc(sizeof(char *) * totallines);
    for (int i = 0; i < totallines; i++)
    {
        all[i] = (char *)malloc(sizeof(char) * 1024);
    }
    fclose(fptr);
    fptr = fopen(finalfile, "r+");
    char buffer2[1024];
    int ct = 0;
    while (fgets(buffer2, sizeof(buffer2), fptr) != NULL)
    {
        strcpy(all[ct++], buffer2);
    }
    if (totallines != 0)
    {
        if (strcmp(all[totallines - 1], input) == 0)
            return;
    }
    fclose(fptr);
    fptr = fopen(finalfile, "w");
    if (totallines < 15)
    {
        for (int i = 0; i < totallines; i++)
        {
            fprintf(fptr, "%s", all[i]);
        }
        if (totallines != 0)
            fprintf(fptr, "\n");

        fprintf(fptr, "%s", input);
    }
    else
    {
        for (int i = 1; i < totallines; i++)
        {
            fprintf(fptr, "%s", all[i]);
        }

        fprintf(fptr, "\n");
        fprintf(fptr, "%s", input);
    }
    for (int i = 0; i < totallines; i++)
    {
        free(all[i]);
    }
    free(all);

    fclose(fptr);
    return;
}

// char *temp = (char *)malloc(sizeof(char) * 1024);
// strcpy(temp, input);
// char *token = strtok(temp, " ");
// int f = 0;
// if (strcmp(pastevents1, token) == 0)
//     f = 1;
// int c = 0;
// while (token != NULL)
// {
//     c++;
//     token = strtok(NULL, " ");
// }
// // printf("%d\n", f);
// if (c == 1 && f)
// {
// }
// if (c == 2 && f)
// {
// }
// else if (c == 3 && f)
// {
// }
// else
// {
//     // char buffer[1024];        // Buffer to store read data
//     // char lastLine[1024] = ""; // Buffer to store the last line
//     // int ct = 0;
//     // while (fgets(buffer, sizeof(buffer), fptr) != NULL)
//     // {
//     //     ct++;
//     //     strcpy(lastLine, buffer); // Store the current line in lastLine
//     // }
//     // if (strcmp(input, lastLine) != 0)
//     // {
//     //     if (ct < 15)
//     //     {
//     //         char **full = (char **)malloc(sizeof(char *) * (ct + 1));
//     //         char buffer[1024];

//     //         // while (fgets(buffer, sizeof(buffer), fptr) != NULL)
//     //         // {

//     //         // }
//     //     }
//     // }
// }