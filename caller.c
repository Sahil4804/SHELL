#include "headers.h"
#include "caller.h"
#include "clean.h"
#include "warp.h"
#include "peek.h"
#include "proclore.h"
#include "pastevents.h"
#include "time.h"
#include "seek.h"
#include "sys/stat.h"
#include "syscall.h"
#include "neonate.h"
#include "iman.h"
#include "fcntl.h"
char *warp = "warp";
char *peek = "peek";
char *proclore = "proclore";
char *pastevents = "pastevents";
char *seek = "seek";
char *activities = "activities";
char *ping = "ping";
char *fg = "fg";
char *bg = "bg";
char *exit1 = "exit";
char *neonate1 = "neonate";
char *iman1 = "iMan";
struct Information *findProcess(struct Information *head, int pidToFind)
{
    struct Information *now = head;

    while (now != NULL)
    {
        int tmeppid = now->pid;
        if (tmeppid == pidToFind)
        {
            return now;
        }
        now = now->nextnode;
    }
    now = NULL;
    return now;
}
void wait_n_switch(int child_pid)
{
    kill(child_pid, SIGCONT);
    int status;
    waitpid(child_pid, &status, WUNTRACED);
    idontchar = child_pid;
    /*   if (tcsetpgrp(STDOUT_FILENO, getpgid(getpid())) == -1) {
       }*/
    if (tcsetpgrp(STDIN_FILENO, getpgid(getpid())) == -1)
    {
        perror("cant get terminal back");
    }

    if (WIFSIGNALED(status))
    {
        listofproc = removeProcess(listofproc, child_pid);
    }
    else if (WIFEXITED(status))
    {
        listofproc = removeProcess(listofproc, child_pid);
    }
    else if (WIFSTOPPED(status))
    {
        printf("\n");
        struct Information *x = findProcess(listofproc, child_pid);
        printf(" %d ", x->pid);
        for (int i = 0; i < x->size; i++)
        {
            printf("%s ", x->processname[i]);
        }

        printf("Stopped\n");
    }
}
void fg_handler(char **tokens, int n)
{
    if (n != 2)
    {
        fprintf(stderr, "fg: invalid format! correct format is fg <job number>\n");
        return;
    }
    int t = (int)strtol(tokens[1], NULL, 10);
    int pid = t;
    if (pid <= 0)
    {
        fprintf(stderr, "Job does not exist \n");

        return;
    }
    wait_n_switch(pid);
}
void bg_handler(char **tokens, int n)
{
    if (n != 2)
    {
        fprintf(stderr, "bg: invalid format! bg <job number>\n");
        return;
    }
    if ((int)strtol(tokens[1], NULL, 10) <= 0)
    {
        fprintf(stderr, "Job does not exist \n");
        return;
    }
    if (kill((int)strtol(tokens[1], NULL, 10), SIGCONT) == -1)
    {
        perror("bg");
        return;
    }
}
struct Information *addProcess(struct Information *head, int pid, char *tokens[], int size)
{
    struct Information *temp = (struct Information *)malloc(sizeof(struct Information));
    if (!temp)
    {
        perror("bad malloc");
        exit(1);
    }
    temp->processname = (char **)malloc(sizeof(char *) * (size + 10));
    for (int i = 0; i < size; i++)
    {
        temp->processname[i] = (char *)malloc(sizeof(char) * 1024);
    }
    for (int i = 0; i < size; i++)
    {
        strcpy(temp->processname[i], tokens[i]);
    }
    temp->size = size;
    temp->nextnode = head;
    temp->pid = pid;
    return temp;
}
struct Information *removeProcess(struct Information *head, pid_t pid)
{
    struct Information *then = (struct Information *)malloc(sizeof(struct Information));
    then = NULL;
    struct Information *now = (struct Information *)malloc(sizeof(struct Information));
    now = head;

    while (now)
    {
        int temppid = now->pid;
        if (temppid == pid)
        {
            if (then != NULL)
            {
                then->nextnode = now->nextnode;
            }
            else
            {
                head = now->nextnode;
            }
            free(now);
            break;
        }
        then = now;
        now = now->nextnode;
    }

    return head;
}
void redirection(char *input, int f, int p[2], int chorus)
{
    input = clean(input);
    if (input == NULL)
        return;
    char temp[1024];
    strcpy(temp, input);
    char *token = strtok(temp, " ");
    char *temp1[1024];
    for (int i = 0; i < 1024; i++)
    {
        temp1[i] = (char *)malloc(sizeof(char) * 1024);
    }
    int c1 = 0;
    int fl = 0;
    int fleft = 0;
    int fright = 0;
    while (token != NULL)
    {
        strcpy(temp1[c1++], token);
        if (strcmp(temp1[c1 - 1], ">") == 0 || strcmp(temp1[c1 - 1], "<") == 0 || strcmp(temp1[c1 - 1], ">>") == 0)
            fl = 1;
        token = strtok(NULL, " ");
    }

    if (fl == 0)
    {
        caller(input, f);
        return;
    }
    else
    {
    }
    char *storenew[100];
    for (int i = 0; i < 100; i++)
    {
        storenew[i] = (char *)malloc(sizeof(char) * 1024);
    }
    char *maybetemp = (char *)malloc(sizeof(char) * 1024);
    strcat(maybetemp, temp1[0]);
    strcat(maybetemp, " ");

    int in = dup(0);
    int out = dup(1);
    int cnew = 0;
    int flagfor = 0;
    for (int i = 1; i < c1; i++)
    {
        if (strcmp(temp1[i], ">") == 0)
        {
            maybetemp[strlen(maybetemp) - 1] = '\0';
            strcpy(storenew[cnew++], maybetemp);
            // printf("%s from here\n", maybetemp);
            strcpy(maybetemp, "");
            int newfile = open(temp1[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (newfile == -1)
            {
                printf("some error occured while opening the file %s\n", temp1[i + 1]);
                return;
            }
            char *finalcaller = (char *)malloc(sizeof(char) * 1e4);
            strcpy(finalcaller, storenew[cnew - 1]);
            close(STDOUT_FILENO);
            dup(newfile);
            close(newfile);
            flagfor = 0;
            caller(finalcaller, 0);
        }
        else if (strcmp(temp1[i], ">>") == 0)
        {
            maybetemp[strlen(maybetemp) - 1] = '\0';
            strcpy(storenew[cnew++], maybetemp);
            strcpy(maybetemp, "");
            int newfile = open(temp1[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (newfile == -1)
            {
                printf("some error occured while opening the file %s\n", temp1[i + 1]);
                return;
            }
            char *finalcaller = (char *)malloc(sizeof(char) * 1e4);
            strcpy(finalcaller, storenew[cnew - 1]);
            close(STDOUT_FILENO);
            dup(newfile);
            close(newfile);
            flagfor = 0;
            caller(finalcaller, 0);
        }
        else if (strcmp(temp1[i], "<") == 0)
        {
            flagfor = 1;
            maybetemp[strlen(maybetemp) - 1] = '\0';
            strcpy(storenew[cnew++], maybetemp);
            // printf("%s from here\n", storenew[cnew - 1]);
            strcpy(maybetemp, "");
            int newfile = open(temp1[i + 1], O_RDONLY, 0644);
            if (newfile == -1)
            {
                printf("some error occured while opening the file %s\n", temp1[i + 1]);
                return;
            }
            // strcpy(temp1[i + 1], temp1[i - 1]);
            in = dup(0);
            close(STDIN_FILENO);
            dup(newfile);
            int flfl = 0;
            for (int j = i; j < i + 3; j++)
            {
                if (strcmp(temp1[j], ">") == 0 || strcmp(temp1[j], ">>") == 0)
                    flfl = 1;
            }

            if (flfl == 0)
            {
                caller(storenew[cnew - 1], f);
            }
            else
            {
                // printf("udta hu\n");
                // flfl = 0;
                int newfile1;
                if (strcmp(temp1[i + 2], ">") == 0)
                    newfile1 = open(temp1[i + 3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                else if (strcmp(temp1[i + 2], ">>") == 0)
                    newfile1 = open(temp1[i + 3], O_WRONLY | O_CREAT | O_APPEND, 0644);

                printf("%s\n", temp1[i + 3]);
                if (newfile1 == -1)
                {
                    printf("some error occured while opening the file %s\n", temp1[i + 3]);
                    return;
                }
                close(STDOUT_FILENO);
                dup(newfile1);
                close(newfile1);
                i += 4;
                caller(storenew[cnew - 1], f);
            }

            close(newfile);
        }
        else
        {
            strcat(maybetemp, temp1[i]);
            strcat(maybetemp, " ");
        }
    }
    if (chorus == 1)
    {
        dup2(in, 0);
        dup2(out, 1);
    }
    return;
}
void childHandler(int signum)
{
    int status;
    int PiD;

    while ((PiD = waitpid(-1, &status, WNOHANG)) > 0)
    {
        Node *child = (Node *)malloc(sizeof(Node));
        if (child == NULL)
        {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }

        child->procPid = PiD;
        child->stat = status;
        child->next = listlist;
        listlist = child;
    }
    return;
}
int pipe_checker(char *input, int f)
{
    if (f)
    {
        printf("[%d]\n", getpid());
    }
    char *temp = (char *)malloc(sizeof(char) * 1024);
    strcpy(temp, input);
    char *token = strtok(temp, " ");
    int f1 = 0;
    int f2 = 0;
    while (token != NULL)
    {
        if (strcmp(token, "|") == 0)
        {
            f1++;
        }
        f2++;
        token = strtok(NULL, " ");
    }
    if (f1 == 0)
    {
        redirection(input, f, NULL, 1);
        return 0;
    }
    char *command[f2 + 10];
    for (int i = 0; i < f2 + 5; i++)
    {
        command[i] = (char *)malloc(sizeof(char) * 1024);
    }
    strcpy(temp, input);
    char *x = strtok(temp, "|");
    int c = 0;
    while (x != NULL)
    {
        strcpy(command[c++], x);
        x = strtok(NULL, "|");
    }
    if (c == 0)
    {
        redirection(input, f, NULL, 1);
        return;
    }
    int then = -1;
    int in = dup(0);
    int out = dup(1);
    int fd[2];
    for (int i = 0; i < c - 1; i++)
    {
        if (pipe(fd) == -1)
        {
            printf("An error has occured creating the pipe\n");
            return;
        }
        int child = fork();
        if (child == -1)
        {
            return;
        }
        else if (child == 0)
        {
            if (then != -1)
            {
                dup2(then, 0);
                close(then);
            }
            char c1[1024], c2[1024];
            strcpy(c1, command[i]);
            strcpy(c2, command[i + 1]);
            dup2(fd[1], 1);
            if (i == c - 2)
                redirection(command[i], 0, fd, 1);
            else
                redirection(command[i], 0, fd, 0);
            exit(0);
        }
        else
        {
            wait(NULL);
            then = fd[0];
            close(fd[1]);
        }
    }
    dup2(out, 1);
    if (then != -1)
    {
        dup2(then, 0);
        fd[0] = then;
        redirection(command[c - 1], f, fd, 1);
        close(then);
    }
    else
    {
        dup2(in, 0);
        redirection(command[c - 1], f, NULL, 1);
    }
    dup2(in, 0);
    dup2(out, 1);
    return 1;
}
int caller(char *input, int f)
{

    input = clean(input);
    int ct = 0;
    char temp[1024];
    char temp22[1024];
    strcpy(temp, input);
    strcpy(temp22, input);
    char *token = strtok(temp, " ");
    token = clean(token);
    if (ss == 0)
    {
        ss = 1;
        pipe_checker(input, f);
        return 0;
    }
    if (strcmp(token, warp) == 0)
    {
        warp_function(input, f);
    }
    else if (strcmp(token, peek) == 0)
    {
        char copy1[1024], copy2[1024];
        strcpy(copy1, input);
        strcpy(copy2, input);
        char *tokens[1000];
        int num = 0;
        tokens[0] = strtok(copy2, " \t\n");
        while (tokens[num] != NULL)
        {
            tokens[++num] = strtok(NULL, " \t");
        }
        peek_function(tokens, num, f);
    }
    else if (strcmp(token, proclore) == 0)
    {
        if (f)
        {
            printf("[%d]\n", getpid());
        }
        proclore_function2(input);
    }
    else if (strcmp(token, pastevents) == 0)
    {
        ct++;
        pastevents_function(input, f);
    }
    else if (strcmp(token, seek) == 0)
    {
        seek_function(input);
    }
    else if (strcmp(token, exit1) == 0)
    {
        // return 0;
        exit(1);
    }
    else if (strcmp(token, fg) == 0)
    {
        char copy1[1024], copy2[1024];
        strcpy(copy1, input);
        strcpy(copy2, input);
        char *tokens[1000];
        int num_tokens = 0;
        tokens[0] = strtok(copy2, " \t\n");
        while (tokens[num_tokens] != NULL)
        {
            tokens[++num_tokens] = strtok(NULL, " \t");
        }
        fg_handler(tokens, num_tokens);
    }
    else if (strcmp(token, bg) == 0)
    {
        char copy1[1024], copy2[1024];
        strcpy(copy1, input);
        strcpy(copy2, input);
        char *tokens[1000];
        int num_tokens = 0;
        tokens[0] = strtok(copy2, " \t\n");
        while (tokens[num_tokens] != NULL)
        {
            tokens[++num_tokens] = strtok(NULL, " \t");
        }
        bg_handler(tokens, num_tokens);
    }
    else if (strcmp(token, ping) == 0)
    {
        char copy1[1024], copy2[1024];
        strcpy(copy1, input);
        strcpy(copy2, input);
        char *tokens[1000];
        int num_tokens = 0;
        tokens[0] = strtok(copy2, " \t\n");
        while (tokens[num_tokens] != NULL)
        {
            tokens[++num_tokens] = strtok(NULL, " \t");
        }
        int signals = atoi(tokens[2]);
        int pid = atoi(tokens[1]);
        printf("Signal %d sent to process with PID %d\n", signals, pid);
        if (kill(pid, signals) == 0)
        {
            printf("Signal %d sent to process with PID %d\n", signals, pid);
        }
        else
        {
            perror("kill");
            return 0;
        }
    }
    else if (strcmp(token, neonate1) == 0)
    {

        neonate(input);
    }
    else if (strcmp(token, activities) == 0)
    {
        struct Information *now = listofproc;
        struct Information *then = NULL;
        while (now != NULL)
        {
            char state = 0;
            int pd;
            char new_name[1024];
            char statee[100];
            int pid = now->pid;
            int job = (pid);
            char location[1024];
            sprintf(location, "/proc/%d/cmdline", pid);
            sprintf(location, "/proc/%d/stat", pid);
            FILE *f = fopen(location, "r");
            if (f != NULL)
            {
                fscanf(f, " %d ", &pd);
                fscanf(f, " %s ", new_name);
                fscanf(f, " %c ", &state);
                if (state == 'R')
                    strcpy(statee, "Running");
                else if (state == 'S')
                    strcpy(statee, "Running");
                else if (state == 'Z')
                    strcpy(statee, "Zombie");
                else if (state == 'T')
                    strcpy(statee, "Stopped");
                else
                    strcpy(statee, "Unknown State");
                int flag = 0;
                if (f == NULL)
                {
                    flag == 1;
                }
                fclose(f);
                printf(" %d ", now->pid);
                for (int i = 0; i < now->size; i++)
                {
                    printf("%s ", now->processname[i]);
                }
                printf("%s\n", statee);
            }
            else
            {

                printf(" %d ", now->pid);
                for (int i = 0; i < now->size; i++)
                {
                    printf("%s ", now->processname[i]);
                }

                printf("finished\n");
                listofproc = removeProcess(listofproc, now->pid);
            }
            then = now;
            now = now->nextnode;
        }
    }
    else if (strcmp(token, iman1) == 0)
    {
        iman(input);
    }
    else
    {
        char *temp1 = (char *)malloc(sizeof(char) * (1024));
        strcpy(temp1, input);
        char *token1 = strtok(temp1, " ");
        int ct = 0;
        while (token1 != NULL)
        {
            ct++;
            token1 = strtok(NULL, " ");
        }
        int st, et;
        st = time(NULL);

        char *ccc;
        ccc = strdup(token);
        char *argv[ct + 1];
        char *token2 = strtok(input, " ");
        int j = 0;
        while (token2 != NULL)
        {
            argv[j] = strdup(token2);
            token2 = strtok(NULL, " ");
            j++;
        }
        argv[j] = NULL;
        int rc = fork();
        idontchar = rc;
        if (rc < 0)
            perror("creating child process failed\n");
        else if (rc == 0)
        {
            
            setpgid(0, 0);
            // printf("%s\n", ccc);
            if (execvp(ccc, argv) == -1)
                printf("invalid ccc\n"), exit(1);
        }
        else if (rc > 0)
        {
            if (!f)
                waitpid(rc, NULL, WUNTRACED);
            else
            {
                strcpy(store1[rc], temp22);
                printf("child with pid [%d] sent to background\n", rc);
            }
            listofproc = addProcess(listofproc, rc, argv, j - 1);
            signal(SIGCHLD, childHandler);
            et = time(NULL);
            int dur = (int)(et - st);
            if (dur > 2 && f == 0)
            {
                ttiimmee = dur;
                strcpy(command, ccc);
            }
        }
    }
    return ct;
}
