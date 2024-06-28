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

void peek_function(char *inputs[], int sizeofinputs, int Flag_background)
{
    int FLAG_A = 0, FLAG_L = 0;
    if (Flag_background)
    {
        printf("%d\n", getpid());
    }

    if (sizeofinputs == 4)
    {
        if (strcmp(inputs[1], "-a") && strcmp(inputs[2], "-a"))
        {
            printf("Invalid ccc1\n");
            return;
        }
        if (strcmp(inputs[1], "-l") && strcmp(inputs[2], "-l"))
        {
            printf("Invalid ccc2\n");
            return;
        }
        FLAG_A = 1;
        FLAG_L = 1;
        calling(inputs[3], FLAG_A, FLAG_L);
    }
    else if (sizeofinputs == 3)
    {

        if (strlen(inputs[1]) == 3)
        {

            if (strcmp(inputs[1], "-a") == 0)
                FLAG_A = 1;
            else if (strcmp(inputs[1], "-l") == 0)
                FLAG_L = 1;
            else if (strcmp(inputs[1], "-al") == 0 || strcmp(inputs[1], "-la") == 0)
            {
                FLAG_L = 1;
                FLAG_A = 1;
            }
            else
            {
                printf("Invalid ccc\n");
                return;
            }
        }
        else if (strlen(inputs[1]) == 2)
        {
            if (strcmp(inputs[1], "-l") == 0)
            {
                FLAG_L = 1;
            }
            else if (strcmp(inputs[1], "-a") == 0)
            {
                FLAG_A = 1;
            }
            else
            {
                printf("Invalid ccc\n");
                return;
            }
        }
        else
        {
            printf("Invalid ccc\n");
            return;
        }
        calling(inputs[2], FLAG_A, FLAG_L);
    }
    else if (sizeofinputs == 2)
    {
        if (strcmp(inputs[1], ".") == 0)
        {
            calling(inputs[1], FLAG_A, FLAG_L);
        }
        else if (strcmp(inputs[1], "~") == 0)
        {
            calling(Pardir, FLAG_A, FLAG_L);
        }

        else if (!strcmp(inputs[1], "-l"))
        {
            calling(".", 0, 1);
        }
        else if (!strcmp(inputs[1], ".."))
        {
            calling(inputs[1], FLAG_A, FLAG_L);
        }
        else if (!strcmp(inputs[1], "-"))
        {
            calling(past, FLAG_A, FLAG_L);
        }
        else if (!strcmp(inputs[1], "-a"))
        {
            calling(".", 1, 0);
        }
        else
        {
            calling(inputs[1], FLAG_A, FLAG_L);
        }
    }
    else if (sizeofinputs == 1)
    {
        calling(".", 0, 0);
    }
    else
    {
        printf("Invalid ccc\n");
        return;
    }
}
