#include "headers.h"
#include "hostname.h"
#include "curdir.h"
#include "pwd.h"
#include "cd.h"
#include "parse.h"
#include "echo.h"
#include "ls.h"
#include "jobs.h"
#include "othercommands.h"
#include "andothercommands.h"
#include "pinfo.h"
#include "history.h"
#include "manageEnviro.h"
#include "printjobs.h"
#include "bgfg.h"
#include "commandhandler.h"

void commandHandler(char **argv, char *line, jobs background[], jobs *fgdetails, char PrevDir[], int *Cstatus)
{
    char *startDir = getenv("PWD");

    int fand = 0, fd, end = 0, i = 0, j = 0, argc;
    char *argv_sub[64];
    int standardIn = dup(STDIN_FILENO);
    int standardOut = dup(STDOUT_FILENO);

    for (int i = 0; argv[i] != NULL; i++)
    {
        if (strcmp(argv[i], "|") == 0 && argv[i + 1] == NULL)
        {
            *Cstatus = 1;
            printf("Invalid syntax\n");
            return;
        }
    }

    while (end == 0)
    {
        int filedes[2];
        j = argc = 0;

        while (strcmp(argv[i], "|") != 0)
        {
            argv_sub[j] = (char *)malloc(100);
            argc++;
            strcpy(argv_sub[j], argv[i]);
            j++;
            i++;
            if (argv[i] == NULL)
            {
                end = 1;
                break;
            }
        }
        if (argv[i] != NULL && strcmp(argv[i], "|") == 0)
            i++;

        argv_sub[j] = NULL;

        if (end == 0)
        {
            pipe(filedes);
            dup2(filedes[1], STDOUT_FILENO);
            close(filedes[1]);
        }
        else
        {
            dup2(standardOut, STDOUT_FILENO);
        }

        for (int i = 0; argv_sub[i] != NULL; i++)
        {
            if (strcmp(argv_sub[i], "<") == 0)
            {
                if (argv_sub[i + 1] != NULL)
                {
                    int fd = open(argv_sub[i + 1], O_RDONLY);
                    if (fd < 0)
                    {
                        dup2(standardIn, STDIN_FILENO);
                        dup2(standardOut, STDOUT_FILENO);
                        fprintf(stderr, "Input file doesn't exist\n");
                        *Cstatus = 1;
                        return;
                    }
                    dup2(fd, STDIN_FILENO);
                    close(fd);
                    free(argv_sub[i]);
                    free(argv_sub[i + 1]);
                    argv_sub[i] = argv_sub[i + 1] = NULL;
                    i++;
                }
                else
                {
                    dup2(standardIn, STDIN_FILENO);
                    dup2(standardOut, STDOUT_FILENO);
                    fprintf(stderr, "Too few arguments\nRequired input format: <command> < <input filename>\n");
                    *Cstatus = 1;
                    return;
                }
            }
            else if (strcmp(argv_sub[i], ">") == 0 || strcmp(argv_sub[i], ">>") == 0)
            {
                if (argv_sub[i + 1] != NULL)
                {
                    if (strcmp(argv_sub[i], ">") == 0)
                        fd = open(argv_sub[i + 1], O_CREAT | O_TRUNC | O_WRONLY, 0644);
                    else
                        fd = open(argv_sub[i + 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
                    if (fd < 0)
                    {
                        dup2(standardIn, STDIN_FILENO);
                        dup2(standardOut, STDOUT_FILENO);
                        *Cstatus = 1;
                        perror("open");
                        return;
                    }
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                    free(argv_sub[i]);
                    free(argv_sub[i + 1]);
                    argv_sub[i] = argv_sub[i + 1] = NULL;
                    i++;
                }
                else
                {
                    dup2(standardIn, STDIN_FILENO);
                    dup2(standardOut, STDOUT_FILENO);
                    fprintf(stderr, "Too few arguments\nRequired output format: <command> > <output filename>\n");
                    *Cstatus = 1;
                    return;
                }
            }
        }
        if (strcmp(argv_sub[0], "&") == 0)
        {
            dup2(standardIn, STDIN_FILENO);
            dup2(standardOut, STDOUT_FILENO);
            fprintf(stderr, "Execvp: No such file or directory\n");
            *Cstatus = 1;
            return;
        }

        if (strcmp(argv_sub[0], "pwd") == 0)
            pwd();
        else if (strcmp(argv_sub[0], "cd") == 0)
        {
            if (argv_sub[1] == NULL)
                chdir(startDir);
            else
                cd(argv_sub[1], startDir, PrevDir, Cstatus);
        }

        else if (strcmp(argv_sub[0], "echo") == 0)
            echo(argv_sub);
        else if (strcmp(argv_sub[0], "ls") == 0)
            ls(argv_sub, argc, Cstatus);
        else if (strcmp(argv_sub[0], "pinfo") == 0)
        {
            if (argv_sub[1] == NULL)
                pinfo(getpid(), Cstatus);
            else
                pinfo(atoi(argv_sub[1]), Cstatus);
        }
        else if (strcmp(argv_sub[0], "history") == 0)
        {
            if (argv_sub[1] == NULL)
                history(10, 1, startDir, line);
            else
                history(atoi(argv_sub[1]), 1, startDir, line);
        }
        else if (strcmp(argv_sub[0], "setenv") == 0)
        {
            manageenviro(argv_sub, 0, Cstatus);
        }
        else if (strcmp(argv_sub[0], "unsetenv") == 0)
        {
            manageenviro(argv_sub, 1, Cstatus);
        }
        else if (strcmp(argv_sub[0], "jobs") == 0)
            printjobs(background);
        else if (strcmp(argv_sub[0], "kjob") == 0)
        {
            if (argv_sub[1] != NULL && argv_sub[2] != NULL && argv_sub[3] == NULL)
                kjob(background, atoi(argv_sub[1]), atoi(argv_sub[2]), Cstatus);
            else
            {
                fprintf(stderr, "Incorrect number of arguments\n");
                *Cstatus = 1;
            }
        }
        else if (strcmp(argv_sub[0], "bg") == 0)
        {
            if (argv_sub[1] != NULL)
                bg(atoi(argv_sub[1]), background, Cstatus);
            else
            {
                printf("Not enough arguments\n");
                *Cstatus = 1;
            }
        }
        else if (strcmp(argv_sub[0], "fg") == 0)
        {
            if (argv_sub[1] != NULL)
                fg(atoi(argv_sub[1]), background, fgdetails, Cstatus);
            else
            {
                printf("Not enough arguments\n");
                *Cstatus = 1;
            }
        }
        else if (strcmp(argv_sub[0], "overkill") == 0)
        {
            for (int i = 0; i < 20; i++)
            {
                if (background[i].pid != -1)
                {
                    kill(background[i].pid, 9);
                }
            }
        }
        else
        {
            for (int i = 0; i < argc; i++)
            {
                if (argv_sub[i] != NULL && strcmp(argv_sub[i], "&") == 0)
                {
                    fand = 1;
                    break;
                }
            }
            if (fand)
            {
                andothercommands(argv_sub, background, standardOut, Cstatus);
            }
            else
            {
                othercommands(argv_sub, fgdetails, background, Cstatus);
            }
        }

        if (end == 0)
        {
            dup2(filedes[0], STDIN_FILENO);
        }
        else
        {
            dup2(standardIn, STDIN_FILENO);
        }
        for (int i = 0; argv_sub[i] != NULL; i++)
            free(argv_sub[i]);
    }
    dup2(standardIn, STDIN_FILENO);
    dup2(standardOut, STDOUT_FILENO);
}