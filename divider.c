#include "headers.h"
#include "clean.h"
#include "peek.h"
#include "caller.h"
#include "pastevents.h"
char *pastcheck = "pastevents";
void divider(char *inp)
{
    char *extra = (char *)malloc(sizeof(char) * 1024);
    strcpy(extra, inp);
    // printf("%s\n", extra);
    inp = clean(inp);
    // printf("from div %s\n", inp);
    char inp2[1024], inp3[1024];
    char *temp;
    strcpy(inp3, inp);
    strcpy(inp2, inp);
    int c = 0;
    temp = strtok(inp, ";&");
    while (temp != NULL)
    {
        temp = clean(temp);
        temp = strtok(NULL, ";&");
        c++;
    }
    int check = 0;
    char *cc[c + 1];
    int i = 0;
    if (c <= 0)
        return;
    char *beg = inp2;
    cc[0] = strtok(inp2, ";&");

    while (cc[i] != NULL && strcmp(cc[i], "") != 0)
    {
        i++;
        cc[i] = strtok(NULL, ";&");
    }
    // printf("%d\n check", check);
    int k = 0;
    int in = dup(0);
    int out = dup(1);
    for (int j = 0; j < c; j++)
    {
        int bg = 0;
        if (inp3[strlen(cc[j]) + (cc[j] - beg)] == '&')
        {
            bg = 1;
        }
        // cc[j] = clean(cc[j]);
        // printf("%s\n", cc[j]);
        ss = 0;
        k += caller(cc[j], bg);
        // dup2(in, 0);
        // close(in);
        // dup2(out, 1);
        // close(out);
    }
    // return;
    if (k == 0)
    {
    // printf("%s %d\n", extra, k);
        store(extra);
    }
}
void divider2(char *inp)
{
    inp = clean(inp);
    // printf("from div %s\n", inp);
    char inp2[1024], inp3[1024];
    char *temp;
    strcpy(inp3, inp);
    strcpy(inp2, inp);
    int c = 0;
    temp = strtok(inp, ";&");
    while (temp != NULL)
    {
        temp = clean(temp);
        temp = strtok(NULL, ";&");
        c++;
    }
    int check = 0;
    char *cc[c + 1];
    int i = 0;
    if (c <= 0)
        return;
    char *beg = inp2;
    cc[0] = strtok(inp2, ";&");

    while (cc[i] != NULL && strcmp(cc[i], "") != 0)
    {
        i++;
        cc[i] = strtok(NULL, ";&");
    }
    int k = 0;
    for (int j = 0; j < c; j++)
    {
        int bg = 0;
        if (inp3[strlen(cc[j]) + (cc[j] - beg)] == '&')
        {
            bg = 1;
        }

        k += caller(cc[j], bg);
    }
}
// printf("%s %d\n", cc[j],bg) ;
// if (strcmp(cc[j], "warp\n") == 0)
// {
//     if (bg == 1)
//         printf("[%d]\n", getpid());
//     printf("%s\n", Pardir);
//     chdir(Pardir);
//     continue;
// }
// if (strcmp(cc[j], "peek\n") == 0)
// {
//     peek_function("peek", bg);
//     continue;
// }