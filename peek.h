#include "headers.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<stdlib.h>
#include<string.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>


void calling(char *dir, int op_a, int op_l);
char *converter(int number, char *perm, int ct1);
int cmpstringp(const void *p1, const void *p2);
void peek_function(char *tokens[], int num_tokens, int proccessid);