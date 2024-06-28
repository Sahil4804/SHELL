#include "headers.h"
#include "extra.h"

#include "peek.h"
#include "headers.h"
#include <dirent.h>
// For EXIT codes and error handling
#include <errno.h>
#include <stdlib.h>
#include "prompt.h"
#include "time.h"
#include "warp.h"
#include "peek.h"
#include "seek.h"
#include "pastevents.h"
#include "unistd.h"
#include "fcntl.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "proclore.h"
#include <pwd.h>
#include <grp.h>
#include "extra.h"
void call1() { printf("%s", "\033[1m\033[33m"); }
void call2() { printf("%s", "\033[1m\033[34m"); }
void call3() { printf("%s", "\033[1m\033[32m"); }
void get(int mode, int tt)
{
    char final[10];
    for (int i = 0; i < 9; i++)
    {
        if (i % 3 == 0)
        {
            if ((mode >> i) & 1)
            {
                final[i] = 'x';
            }
            else
            {
                final[i] = '-';
            }
        }
        else if (i % 3 == 1)
        {
            if ((mode >> i) & 1)
            {
                final[i] = 'w';
            }
            else
            {
                final[i] = '-';
            }
        }
        else
        {
            if ((mode >> i) & 1)
            {
                final[i] = 'r';
            }
            else
            {
                final[i] = '-';
            }
        }
    }
    for (int i = 8; i >= 0; i--)
    {

        printf("%s", "\033[1m\033[0m");
        printf("%c", final[i]);
    }
    printf(" ");

    return;
}
char *months[] = {"Jan", "Feb", "March", "April", "May", "June", "July", "Aug", "Sept", "Oct", "Nov", "Dec"};
int cmp(const void *a, const void *b)
{
    const char **str_a = (const char **)a;
    const char **str_b = (const char **)b;
    return strcmp(*str_a, *str_b);
}
void calling(char *dir, int oper_hidden, int oper_details)
{

    if (!strcmp(dir, "-"))
    {
        strcpy(dir, past);
    }
    if (!strcmp(dir, "~"))
    {
        strcpy(dir, Pardir);
    }
    DIR *dh = opendir(dir);
    int nonoofiles = 0;

    if (dh == NULL)
    {
        if (errno = ENOENT)
        {
            perror("Directory doesn't exist");
        }
        else
        {
            perror("Unable to read directory");
        }
        // exit(EXIT_FAILURE);
        return;
    }
    int c = 0;
    while (readdir(dh) != NULL)
    {
        c++;
    }
    nonoofiles = c;

    closedir(dh);
    dh = opendir(dir);
    int counterforfile = 0;
    struct dirent *d;
    int tottot = 0;
    char **files_sort = (char **)malloc(sizeof(char *) * nonoofiles);

    for (int i = 0; i < nonoofiles; i++)
        files_sort[i] = (char *)malloc(sizeof(char) * 1024);

    while ((d = readdir(dh)) != NULL)
    {
        if (d->d_name[0] == '.' && oper_hidden == 0)
            continue;
        strcpy(files_sort[counterforfile++], d->d_name);
        struct stat data;
        char *addr = (char *)malloc(sizeof(char) * 1024);
        strcpy(addr, dir);
        strcat(addr, "/");
        strcat(addr, d->d_name);
        if (lstat(addr, &data) == -1)
        {
            printf("%s\n", d->d_name);
            perror("Error getting stat struct");
            return;
        }
        tottot += data.st_blocks;
    }
    qsort(files_sort, counterforfile, sizeof(char *), cmp);
    if (oper_details)
        printf("total %d\n", tottot >> 1);
    for (int i = 0; i < counterforfile; i++)
    {

        char *addr = (char *)malloc(sizeof(char) * 1024);
        strcpy(addr, dir);
        strcat(addr, "/");
        strcat(addr, files_sort[i]);

        struct stat data;
        int counterforfile = 0;
        int temp = lstat(addr, &data);
        if (temp == -1)
        {
            printf("%s\n", files_sort[i]);
            perror("Error getting stat struct");
            return;
        }

        if (oper_details)
        {
            int color = 0, color2 = 0, color3 = 0;
            if (data.st_mode & __S_IFDIR)
            {
                color = 1;
            }
            else if (data.st_mode & S_IXUSR)
            {
                color2 = 1;
            }
            else if (S_ISLNK(data.st_mode))
            {
                color3 = 1;
            }
            if (color3)
            {
                call1();
            }
            else if (color)
            {

                call2();
            }
            else if (color2)
            {
                call3();
            }
            else
            {
                printf("%s", "\033[1m\033[0m");
            }

            char modeStr[20];
            mode_t mode = data.st_mode;

            snprintf(modeStr, sizeof(modeStr), "%c%c%c%c%c%c%c%c%c%c",
                     (S_ISDIR(mode)) ? 'd' : '-',
                     (mode & S_IRUSR) ? 'r' : '-',
                     (mode & S_IWUSR) ? 'w' : '-',
                     (mode & S_IXUSR) ? 'x' : '-',
                     (mode & S_IRGRP) ? 'r' : '-',
                     (mode & S_IWGRP) ? 'w' : '-',
                     (mode & S_IXGRP) ? 'x' : '-',
                     (mode & S_IROTH) ? 'r' : '-',
                     (mode & S_IWOTH) ? 'w' : '-',
                     (mode & S_IXOTH) ? 'x' : '-');

            struct tm lmod;
            time_t timeofthefile = data.st_mtime;
            localtime_r(&timeofthefile, &lmod);
            char *name = (char *)malloc(sizeof(char) * 5);

            strcpy(name, months[lmod.tm_mon]);
            printf("%s %4ld %s %s %7ld %s %02d %02d:%02d %s ", modeStr, data.st_nlink, getpwuid(data.st_uid)->pw_name, getgrgid(data.st_gid)->gr_name, data.st_size, name, lmod.tm_mday, lmod.tm_hour, lmod.tm_min, files_sort[i]);
            printf("%s\n", "\033[1m\033[0m");
        }
        else
        {
            int color = 0, color2 = 0, color3 = 0;
            if (data.st_mode & __S_IFDIR)
            {
                color = 1;
            }
            else if (data.st_mode & S_IXUSR)
            {
                color2 = 1;
            }
            else if (S_ISLNK(data.st_mode))
            {
                color3 = 1;
            }
            if (color2)
            {
                printf("%s", "\033[1m\033[32m");
            }
            else if (color3)
            {
                printf("%s", "\033[1m\033[33m");
            }
            else if (color)
            {
                printf("%s", "\033[1m\033[34m");
            }
            else
            {
                printf("%s", "\033[1m\033[0m");
            }
            printf("%s\n", files_sort[i]);
            printf("%s", "\033[1m\033[0m");
        }
    }
    for (int i = 0; i < counterforfile; i++)
        free(files_sort[i]);
    closedir(dh);
    free(d);
}
