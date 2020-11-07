#include "cd.h"
#include "headers.h"

void change(char *relative, char *path, int start, int *Cstatus)
{
    int len = strlen(path);
    if (len > 2)
    {
        for (int i = start; i < len; i++)
            relative[i] = path[i];
        if (chdir(relative) == -1)
        {
            *Cstatus = 1;
            perror("chdir");
        }
    }
}

void cd(char *path, char *home, char prev[], int *Cstatus)
{

    char relative[1024] = "./", root = '/', dot = '.', tilde = '~', dash = '-';

    if (path[0] == dash)
    {
        char temp[100];
        strcpy(temp, prev);
        prev = getcwd(prev, 100);
        printf("%s\n", temp);
        chdir(temp);
    }
    else
    {
        prev = getcwd(prev, 100);
        if (path[0] == tilde)
        {
            if (chdir(home) == -1)
            {
                *Cstatus = 1;
                perror("chdir");
                return;
            }
            change(relative, path, 2, Cstatus);
        }

        else if (path[0] == dot && path[1] != dot)
            change(relative, path, 0, Cstatus);
        else if (chdir(path) == -1)
        {
            *Cstatus = 1;
            perror("chdir");
        }
    }
}