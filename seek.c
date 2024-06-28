#include "headers.h"
#include "seek.h"
#include "headers.h"
#include "clean.h"
#include "peek.h"
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include "time.h"

int joe = 0;
int cntf = 0;
int cntd = 0;
char ans[1024];
void removeExtension(char *filename)
{
    char *dot = strrchr(filename, '.');
    if (dot != NULL)
    {
        *dot = '\0';
    }
}
void find(char *tofind, char *wheretofind, int ef, int df, int ff)
{
    if (ff == 1 && df == 1)
    {
        printf("Invalid flags!\n");
        joe = 1;
        return;
    }
    // printf("%s\n", wheretofind);
    // printf("%s %s ,%d %d %d", tofind, wheretofind, ef, df, ff);
    if (ff == 1)
    {

        DIR *dir = opendir(wheretofind);
        if (dir == NULL)
        {
            if (errno == EACCES)
            {
                perror("Missing permission for task\n");
            }
            else
            {
                perror("Error opening file\n");
            }
            return;
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }

            char path[1024];
            snprintf(path, sizeof(path), "%s/%s", wheretofind, entry->d_name);

            struct stat statbuf;
            if (stat(path, &statbuf) == -1)
            {
                perror("Error getting file information");
                continue;
            }

            if (S_ISDIR(statbuf.st_mode))
            {
                if (strcmp(entry->d_name, tofind) == 0)
                {
                    cntd++;
                }
                find(tofind, path, ef, df, ff); // Recurse into subdirectory
            }
            else if (S_ISREG(statbuf.st_mode))
            {
                char *temp = (char *)malloc(sizeof(char) * 1024);
                strcpy(temp, entry->d_name);
                removeExtension(temp);
                if (strcmp(entry->d_name, tofind) == 0 || strcmp(temp, tofind) == 0)
                {
                    cntf++;
                    joe = 1;
                    strcpy(ans, path);

                    printf("%s\n", path);
                }
            }
        }

        closedir(dir);
    }
    else if (df == 1)
    {
        DIR *dir = opendir(wheretofind);
        if (dir == NULL)
        {
            if (errno == EACCES)
            {
                perror("Missing permission for task\n");
            }
            else
            {
                perror("Error opening file\n");
            }
            return;
        }

        struct dirent *entry;
        char path[1024];
        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }

            snprintf(path, sizeof(path), "%s/%s", wheretofind, entry->d_name);

            struct stat statbuf;
            if (stat(path, &statbuf) == -1)
            {
                printf("%s\n", path);
                perror("Error getting file information");
                continue;
            }

            if (S_ISDIR(statbuf.st_mode))
            {
                if (strcmp(entry->d_name, tofind) == 0)
                {
                    strcpy(ans, path);
                    printf("%s\n", path);
                    cntd++;
                    joe = 1;
                }
                find(tofind, path, ef, df, ff); // Recurse into subdirectory
            }
            else if (S_ISREG(statbuf.st_mode))
            {
                char *temp = (char *)malloc(sizeof(char) * 1024);
                strcpy(temp, entry->d_name);
                removeExtension(temp);
                if (strcmp(entry->d_name, tofind) == 0 || strcmp(temp, tofind) == 0)
                {
                    cntf++;
                }
            }
        }
        closedir(dir);
    }
    // printf("%d %d\n", cntf, cntd);
    return;
}

void seek_function(char *input)
{
    char *temp = (char *)malloc(sizeof(char) * 1024);
    strcpy(temp, input);
    char *token = strtok(temp, " ");
    int c = 0;
    // printf("%s\n", input);
    while (token != NULL)
    {
        token = strtok(NULL, " ");
        c++;
    }
    // printf("%d\n", c);
    if (c == 2)
    {
        char *token1 = (char *)malloc(sizeof(char) * 1024);
        token1 = strtok(input, " ");
        char **store = (char **)malloc(sizeof(char *) * (c + 1));
        for (int i = 0; i <= c; i++)
        {
            store[i] = (char *)malloc(sizeof(char) * 1024);
        }
        int ct = 0;
        while (token1 != NULL)
        {
            token1 = strtok(NULL, " ");
            if (token1 != NULL)
                strcpy(store[ct++], token1);
        }
        char *CurrentDir = (char *)malloc(sizeof(char) * (1024));
        getcwd(CurrentDir, 1024);
        find(store[0], CurrentDir, 0, 1, 0);
        find(store[0], CurrentDir, 0, 0, 1);
    }
    else if (c == 3)
    {
        char *token1 = (char *)malloc(sizeof(char) * 1024);
        token1 = strtok(input, " ");
        char **store = (char **)malloc(sizeof(char *) * (c + 1));
        for (int i = 0; i <= c; i++)
        {
            store[i] = (char *)malloc(sizeof(char) * 1024);
        }
        int ct = 0;
        while (token1 != NULL)
        {
            token1 = strtok(NULL, " ");
            if (token1 != NULL)
                strcpy(store[ct++], token1);
        }
        find(store[0], store[1], 0, 0, 0);
    }
    else if (c == 4)
    {
        char *token1 = (char *)malloc(sizeof(char) * 1024);
        token1 = strtok(input, " ");
        char **store = (char **)malloc(sizeof(char *) * (c + 1));
        for (int i = 0; i <= c; i++)
        {
            store[i] = (char *)malloc(sizeof(char) * 1024);
        }
        int ct = 0;
        while (token1 != NULL)
        {
            token1 = strtok(NULL, " ");
            if (token1 != NULL)
                strcpy(store[ct++], token1);
        }
        if (strcmp(store[0], "-f") == 0)
        {
            find(store[1], store[2], 0, 0, 1);
        }
        else if (strcmp(store[0], "-e") == 0)
        {
            find(store[1], store[2], 1, 0, 0);
        }
        else if (strcmp(store[0], "-d") == 0)
        {
            find(store[1], store[2], 0, 1, 0);
        }
        else
        {
            printf("Invalid ccc\n");
            return;
        }
    }
    else if (c == 5)
    {
        int ef = 0;
        int df = 0;
        int ff = 0;
        char *token1 = (char *)malloc(sizeof(char) * 1024);
        token1 = strtok(input, " ");
        char **store = (char **)malloc(sizeof(char *) * (c + 1));
        for (int i = 0; i <= c; i++)
        {
            store[i] = (char *)malloc(sizeof(char) * 1024);
        }
        int ct = 0;
        while (token1 != NULL)
        {
            token1 = strtok(NULL, " ");
            if (token1 != NULL)
                strcpy(store[ct++], token1);
        }
        if (strcmp(store[0], "-f") == 0)
        {
            ff = 1;
        }
        else if (strcmp(store[0], "-e") == 0)
        {
            ef = 1;
        }
        else if (strcmp(store[0], "-d") == 0)
        {
            df = 1;
        }
        else
        {
            printf("Invalid ccc\n");
            return;
        }
        if (strcmp(store[1], "-f") == 0)
        {
            ff = 1;
        }
        else if (strcmp(store[1], "-e") == 0)
        {
            ef = 1;
        }
        else if (strcmp(store[1], "-d") == 0)
        {
            df = 1;
        }
        else
        {
            printf("Invalid ccc\n");
            return;
        }
        find(store[2], store[3], ef, df, ff);
        // printf("%d %d\n", cntd, cntf);
        if (cntd == 1 && ef == 1 && df == 1 && cntf == 0)
        {
            char *CurrentDir = (char *)malloc(sizeof(char) * (1024));
            getcwd(CurrentDir, 1024);
            strcpy(past, CurrentDir);
            chdir(ans);
        }
        else if (cntd == 0 && ef == 1 && ff == 1 && cntf == 1)
        {
            // printf("yes");
            // printf("%s yeh hai\n", ans);
            FILE *fptr = fopen(ans, "r");
            if (fptr == NULL)
            {
                printf("Cannot open file \n");
                exit(0);
            }
            c = fgetc(fptr);
            while (c != EOF)
            {
                printf("%c", c);
                c = fgetc(fptr);
            }
            printf("\n");
            fclose(fptr);
        }
        cntd = 0;
        cntf = 0;
    }
    else
    {
        char *token1 = (char *)malloc(sizeof(char) * 1024);
        token1 = strtok(input, " ");
        char **store = (char **)malloc(sizeof(char *) * (c + 1));
        for (int i = 0; i <= c; i++)
        {
            store[i] = (char *)malloc(sizeof(char) * 1024);
        }
        int ct = 0;
        while (token1 != NULL)
        {
            token1 = strtok(NULL, " ");
            if (token1 != NULL)
                strcpy(store[ct++], token1);
        }
        for (int i = 0; i < 3; i++)
        {
            if (strcmp(store[i], "-f") && strcmp(store[i], "-e") && strcmp(store[i], "-d"))
            {
                printf("Invalid ccc\n");
                return;
            }
        }

        find(store[3], store[4], 1, 1, 1);
    }
    if (joe == 0)
    {
        printf("Not found!\n");
    }
    joe = 0;
}
