#include "headers.h"
#include "jobs.h"
#include "andothercommands.h"

void andothercommands(char **argv, jobs background[], int standardOut, int *Cstatus)
{
    int foundAnd = 0, max = 0, i = 0;
    for (i = 0; argv[i + 1] != NULL; i++)
    {
        if (strcmp(argv[i], "&") == 0)
        {
            foundAnd = 1;
            argv[i] = argv[i + 1];
        }
        else if (foundAnd)
            argv[i] = argv[i + 1];
    }
    if (strcmp(argv[i], "&") == 0)
    {
        free(argv[i]);
        argv[i] = NULL;
    }
    int index = -1;
    for (int i = 0; i < 20; i++)
    {
        if (background[i].pid == -1)
        {
            index = i;
            break;
        }
    }

    for (int i = 0; i < 20; i++)
        if (max < background[i].job_id)
            max = background[i].job_id;

    if (index != -1)
    {
        int pid = fork();
        if (pid < 0)
        {
            *Cstatus = 1;
            perror("fork");
            return;
        }
        else if (pid == 0)
        {
            signal(SIGTSTP, SIG_DFL);
            signal(SIGINT, SIG_DFL);
            if (setpgid(0, 0) < 0)
            {
                perror("Setpgid");
                exit(0);
            }
            if (execvp(argv[0], argv) == -1)
                perror("execvp");
            exit(0);
        }
        else
        {
            dup2(standardOut, STDOUT_FILENO);
            printf("%s started with pid: %d\n", argv[0], pid);
            background[index].pid = pid;
            strcpy(background[index].name, argv[0]);
            background[index].job_id = max + 1;
        }
    }
    else
    {
        printf("Too many background processes already running\n");
    }
}