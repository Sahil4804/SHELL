#include "headers.h"

char *clean(char *line)
{
    // leading
    int t = 0;
    for (int i = 0; i < strlen(line); i++)
    {
        if (line[i] == ' ' || line[i] == '\t' || line[i] == '\n')
        {
            t++;
        }
        else
        {
            break;
        }
    }
    // printf("%d\n", t);
    for (int i = 0; i < t; i++)
    {
        line++;
    }
    // printf("%s\n", line);
    //  trailing
    for (int i = strlen(line) - 1; i >= 0; i--)
    {
        if (line[i] == ' ' || line[i] == '\t' || line[i] == '\n')
        {
            line[i] = '\0';
        }
        else
        {
            break;
        }
    }
    // printf("%s", line);
    return line;
}