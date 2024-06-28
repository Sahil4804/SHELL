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
#include <termios.h>

void neonate(char *command)
{
    char copy1[1024], copy2[1024];
    strcpy(copy1, command);
    strcpy(copy2, command);
    char *Command[1000];
    int num_Command = 0;
    Command[0] = strtok(copy2, " \t\n");
    while (Command[num_Command] != NULL)
    {
        Command[++num_Command] = strtok(NULL, " \t");
    }

    char c;
    struct termios original_termios;
    tcgetattr(STDIN_FILENO, &original_termios);
    struct termios raw_termios = original_termios;
    raw_termios.c_lflag &= ~(ICANON | ECHO);
    raw_termios.c_cc[VMIN] = 0;
    raw_termios.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_termios);
    int start;
    time(&start);
    int end;
    int counter = 0;
    while (1)
    {
        time(&end);
        int diff = end - start;
        if (diff > counter * atoi(Command[2]))
        {
            FILE *f = fopen("/proc/loadavg", "r");
            char data[1024];
            if (f == NULL)
            {
                fprintf(stderr, "Can't open /proc/loadavg file\n");
                break;
            }
            fgets(data, 1024, f);
            char *val = strtok(data, " ");
            for (int i = 0; i < 4; i++)
            {
                val = strtok(NULL, " ");
            }
            printf("%s", val);
            counter++;
        }

        if (read(STDIN_FILENO, &c, 1) == -1)
        {
            perror("read");
            break;
        }
        if (c == 'x')
        {
            c = ' ';
            printf("you pressed the key x :) \n");
            break;
        }

    }

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);

    return;
}