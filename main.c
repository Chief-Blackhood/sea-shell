#include "headers.h"
#include "jobs.h"
#include "commandhandler.h"
#include "parse.h"
#include "hostname.h"
#include "curdir.h"
#include "history.h"

#define startDir getenv("PWD")

jobs background[20], fgdetails;
int no_print_prompt = 0, Cstatus = -1;

void signalHandler_child(int p)
{
    fflush(stdout);
    for (int i = 0; i < 20; i++)
    {
        if (background[i].pid != -1)

        {
            int status = -1;
            if (waitpid(background[i].pid, &status, WNOHANG) == -1)
            {
                printf("Some error occurred in waitpid for pid: %d\n", background[i].pid);
                return;
            }

            if (WIFEXITED(status))
                fprintf(stderr, "\n%s with pid %d exited normally. \nExit status: %d\n", background[i].name, background[i].pid, WEXITSTATUS(status));
            else if (WIFSIGNALED(status))
                fprintf(stderr, "\n%s with pid %d exited by receiving a signal. \nExit status: %d\n", background[i].name, background[i].pid, WTERMSIG(status));
            else if (WIFSTOPPED(status))
                fprintf(stderr, "\n%s with pid %d stopped by receiving a signal. \nStopping status: %d\n", background[i].name, background[i].pid, WSTOPSIG(status));

            if (WIFEXITED(status) || WIFSIGNALED(status))
            {
                background[i].pid = -1;
                strcpy(background[i].name, "\0");
                background[i].job_id = -1;
                //no_print_prompt = 1;
                hostname(&Cstatus);
                curdir(startDir, &Cstatus);
                fflush(stderr);
                fflush(stdout);
            }
        }
    }
}

void signalHandler_term(int p)
{
    //no_print_prompt = 1;
    printf("\n");
    hostname(&Cstatus);
    curdir(startDir, &Cstatus);
    fflush(stdout);
}

int main()
{
    char PrevDir[100];
    strcpy(PrevDir, getenv("PWD"));
    char *line = NULL;
    size_t len = 0;
    ssize_t lineSize = 0;
    char **command, **argv;
    char *temp;

    const char delimiter[2] = ";";
    int index, k, i, j, ffirstPosition;
    char buf[1024], buf1[100];

    command = (char **)malloc(64 * sizeof(char *));
    argv = (char **)malloc(64 * sizeof(char *));

    for (int i = 0; i < 20; i++)
    {
        fgdetails.pid = background[i].pid = -1;
        strcpy(background[i].name, "\0");
        strcpy(fgdetails.name, "\0");
        fgdetails.job_id = background[i].job_id = -1;
    }

    signal(SIGCHLD, signalHandler_child);
    signal(SIGINT, signalHandler_term);
    //signal(SIGTSTP, SIG_IGN);
    signal(SIGTSTP, signalHandler_term);

    while (1)
    {

        i = j = ffirstPosition = 0;
        fflush(stdout);
        if (no_print_prompt == 0)
        {
            hostname(&Cstatus);
            curdir(startDir, &Cstatus);
            Cstatus = 0;
        }
        else
        {
            no_print_prompt = 0;
        }

        lineSize = getline(&line, &len, stdin);
        if (no_print_prompt == 1)
            no_print_prompt = 0;
        if (lineSize == -1)
        {
            printf("\n");
            free(line);
            exit(0);
        }

        history(0, 0, startDir, line);
        line[lineSize - 1] = ';';
        for (int i = 0; i < lineSize; i++)
        {
            if (line[i] == ' ' && ffirstPosition == 0)
                ;
            else if (line[i] != ' ')
            {
                buf1[j++] = line[i];
                ffirstPosition = 1;
            }
            else
                buf1[j++] = line[i];
        }
        buf1[j] = '\0';
        temp = strtok(buf1, delimiter);

        while (temp != NULL)
        {
            command[i] = (char *)malloc(200);
            strcpy(command[i], temp);
            i++;
            temp = strtok(NULL, delimiter);
        }
        command[i] = NULL;
        for (int m = 0; command[m] != NULL; m++)
        {
            index = k = 0;
            parse(command[m], argv);
            for (int j = 0; argv[0] != NULL; j++)
            {
                if (argv[0][j] == ' ')
                    index++;
                else
                    break;
            }
            for (int j = index; argv[0][j] != '\0'; j++, k++)
            {
                buf[k] = argv[0][j];
            }
            buf[k] = '\0';
            strcpy(argv[0], buf);
            if (strcmp(argv[0], "quit") == 0)
            {
                for (int i = 0; argv[i] != NULL; i++)
                    free(argv[i]);
                for (int i = 0; command[i] != NULL; i++)
                    free(command[i]);
                free(command);
                free(argv);
                free(line);
                return 0;
            }

            commandHandler(argv, line, background, &fgdetails, PrevDir, &Cstatus);
            for (int i = 0; argv[i] != NULL; i++)
                free(argv[i]);
        }
        for (int i = 0; command[i] != NULL; i++)
            free(command[i]);
    }
}
