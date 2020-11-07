#include "jobs.h"
#include "curdir.h"
#include "headers.h"

void curdir(char *startDir, int *Cstatus)
{
    char curDir[1024];
    if (getcwd(curDir, 1024) == NULL)
    {
        *Cstatus = 1;
        perror("getcwd");
        return;
    }
    int fsame = 1;
    if (strlen(curDir) < strlen(startDir))
    {
        printf("\033[1;34m");
        printf("%s", curDir);
        printf("\033[0m");
        printf("> ");
    }

    else
    {
        for (int i = 0; i < strlen(curDir) && i < strlen(startDir); i++)
        {
            if (curDir[i] != startDir[i])
            {
                fsame = 0;
                break;
            }
        }
        if (fsame)
        {
            printf("\033[1;34m");
            printf("~");
            for (int i = strlen(startDir); i < strlen(curDir); i++)
                printf("%c", curDir[i]);
            printf("\033[0m");
            printf("> ");
        }
        else
        {
            printf("\033[1;34m");
            printf("%s", curDir);
            printf("\033[0m");
            printf("> ");
        }
    }
}