#include "headers.h"
#include "clean.h"
#include "warp.h"
//  int chdir(const char *path);

void warp_function(char *input, int f)
{
    if (f)
    {
        printf("[%d]\n", getpid());
    }
    int c = 0;
    char *store[1000];
    for (int i = 0; i < 1000; i++)
    {
        store[i] = (char *)malloc(sizeof(char) * 1024);
    }
    char temp[1024];
    strcpy(temp, input);
    char *token = strtok(temp, " ");
    while (token != NULL)
    {
        token = strtok(NULL, " ");
        if (token != NULL)
        {
            if (token[0]=='~')
            {
                strcpy(store[c], Pardir);
                for (int i = 1; i < strlen(token); i++)
                {
                    store[c][strlen(Pardir) + i - 1] = token[i];
                }
                // printf("%s\n", store[c]);
            }
            else
                store[c] = token;
            c++;
        }
    }
    if (c == 0)
    {
        char *CurrentDir = (char *)malloc(sizeof(char) * (1024));
        getcwd(CurrentDir, 1024);
        chdir(Pardir);
        strcpy(past, CurrentDir);
        printf("%s\n", CurrentDir);
    }

    for (int i = 0; i < c; i++)
    {
        char *CurrentDir = (char *)malloc(sizeof(char) * (1024));
        getcwd(CurrentDir, 1024);

        if (strlen(store[i]) == 0)
            continue;
        store[i] = clean(store[i]);
        if (strcmp(store[i], "..") == 0)
        {
            strcpy(past, CurrentDir);
            if(chdir(store[i])==-1){
                printf("The directory does not exist\n");
                return;

            }
        }
        else if (strcmp(store[i], "~") == 0)
        {
            strcpy(past, CurrentDir);
            chdir(Pardir);
        }
        else if (strcmp(store[i], "-") == 0)
        {
            chdir(past);
            strcpy(past, CurrentDir);
        }
        else if (strcmp(store[i], "") == 0)
        {
            chdir(Pardir);
            
            strcpy(past, CurrentDir);
        }
        else if (strcmp(store[i], " ") == 0)
        {
            chdir(Pardir);
            strcpy(past, CurrentDir);
        }
        else
        {
            if(chdir(store[i])==-1){
                printf("The directory does not exist\n");
                return;
            }
            strcpy(past, CurrentDir);
        }
        getcwd(CurrentDir, 1024);
        printf("%s\n", CurrentDir);
    }
    return;
}
