
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

#ifndef __PROMPT_H
#define __PROMPT_H

extern char *Pardir;
extern char *past;
extern char *command;
extern int ttiimmee;
void prompt(char *Pardir);
extern int ss;
typedef struct Node
{
    int procPid;
    int stat;
    struct Node *next;
} Node;
extern char **store1;
extern Node *listlist;
struct Information
{
    int pid;
    char **processname;
    int status;
    int size;
    struct Information *nextnode;
};
extern struct Information *listofproc;
struct Information *removeProcess(struct Information *head, pid_t pid);
extern int idontchar;
#endif