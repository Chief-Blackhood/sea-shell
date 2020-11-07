#include "headers.h"
#include "jobs.h"
#include "othercommands.h"

void othercommands(char **argv, jobs *fgdetails, jobs background[], int *Cstatus)
{
    int pid;
    int status;

    if ((pid = fork()) < 0)
    {
        perror("Fork");
    }
    else if (pid == 0)
    {
        signal(SIGTSTP, SIG_DFL);
        signal(SIGINT, SIG_DFL);
        setpgid(0, 0);
        if (execvp(argv[0], argv) == -1)
        {
            perror("Execvp");
            exit(-1);
        }
        exit(0);
    }
    else
    {
        fgdetails->pid = pid;
        strcpy(fgdetails->name, argv[0]);
        signal(SIGTTOU, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        tcsetpgrp(0, pid);
        waitpid(pid, &status, WUNTRACED);
        tcsetpgrp(0, getpid());
        signal(SIGTTOU, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        if (WIFSTOPPED(status) || WEXITSTATUS(status))
            *Cstatus = 1;
        if (WIFSTOPPED(status))
        {
            setpgid(pid, pid);
            int max = 0;
            for (int i = 0; i < 20; i++)
                if (max < background[i].job_id)
                    max = background[i].job_id;

            int index = -1;
            for (int i = 0; i < 20; i++)
            {
                if (background[i].pid == -1)
                {
                    index = i;
                    break;
                }
            }
            if (index == -1)
            {
                printf("Already too many background process running thus terminating the foreground process instead\n");
                kill(fgdetails->pid, 9);
            }
            else
            {
                if (fgdetails->pid != -1)
                {
                    background[index].pid = fgdetails->pid;
                    strcpy(background[index].name, fgdetails->name);
                    background[index].job_id = max + 1;
                    fgdetails->pid = -1;
                    strcpy(fgdetails->name, "\0");
                }
            }
        }
        return;
    }
}