#include "headers.h"
#include "prompt.h"
#include "clean.h"
#include "divider.h"
#include "caller.h"
#include "peek.h"
#include "proclore.h"
#include "sys/wait.h"
#include "sys/types.h"
#include <signal.h>
char *Pardir;
char *past;
int ttiimmee;
char *command;
char **store1;
int idontchar = 0;
Node *listlist = NULL;
struct Information *listofproc = NULL;
int ss = 0;
void custsig(int signum)
{
    return;
}
void sina(int signum)
{
    if (idontchar == 0)
        return;
    int targetPID = idontchar;
    if (kill(targetPID, SIGSTOP) == -1)
    {
        perror("kill");
        // exit(1);
    }

    idontchar = 0;

    // Add any additional actions you want to perform here
}
int main()
{

    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    if (signal(SIGINT, custsig) == SIG_ERR)
    {
        perror("signal");
        return 1;
    }
    if (signal(SIGTSTP, sina) == SIG_ERR)
    {
        perror("signal");
        // exit(1);
    }
    printf("\n\n");
    printf("\033[H\033[J");
    Pardir = (char *)malloc(sizeof(char) * (1024));
    past = (char *)malloc(sizeof(char) * (1024));
    command = (char *)malloc(sizeof(char) * (1024));
    getcwd(Pardir, 1024);
    getcwd(past, 1024);
    int n = 1e5;
    store1 = (char **)malloc(sizeof(char *) * n);
    for (int i = 0; i < n; i++)
    {
        store1[i] = (char *)malloc(sizeof(char) * (1024));
    }
    while (1)
    {
        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt(Pardir);
        ss = 0;
        char *input = (char *)malloc(sizeof(char) * (4096));
        if (fgets(input, 4096, stdin) == NULL)
        {
            ss = 0;
            while (listofproc != NULL)
            {
                struct Information *x = listofproc;
                listofproc = listofproc->nextnode;
                if (kill(x->pid, 9) != 0)
                {
                    perror("kill");
                    return 1;
                }
                else
                {
                    printf("Signal %d sent to process with PID %d\n", 9, x->pid);
                    // printf("Mar gaya\n");
                }
                // kill(x->pid, 9);
                listofproc = removeProcess(listofproc, x->pid);
            }

            // printf("\n");
            exit(0);
        }
        Node *currentNode = listlist;
        while (currentNode != NULL)
        {
            if ((((currentNode->stat) & 0x7f) == 0))
            {
                printf("process %s  [PID]  : %d exited normally with status %d\n", store1[currentNode->procPid], currentNode->procPid, WEXITSTATUS(currentNode->stat));
                store1[currentNode->procPid] = NULL;
            }
            else if ((((signed char)(((currentNode->stat) & 0x7f) + 1) >> 1) > 0))
            {
                printf("process %s [PID]  : %d exited abnormally due to signal %d\n", store1[currentNode->procPid], currentNode->procPid, WTERMSIG(currentNode->stat));
                store1[currentNode->procPid] = NULL;
            }
            Node *temp = currentNode;
            currentNode = currentNode->next;
            free(temp);
        }
        listlist = NULL;
        input = clean(input);
        divider(input);
    }
    printf("\n\n");
}
