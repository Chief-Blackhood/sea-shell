#include "headers.h"
#include "jobs.h"
#include "bgfg.h"

void bg(int jobnum, jobs background[], int *Cstatus)
{
    int pid = -1;
    for (int i = 0; i < 20; i++)
    {
        if (background[i].job_id == jobnum)
        {
            pid = background[i].pid;
            break;
        }
    }
    if (pid == -1)
    {
        printf("Process doesn't exists in the background\n");
        return;
    }
    if (kill(pid, SIGCONT) < 0)
    {
        *Cstatus = 1;
        perror("error in sending the signal");
        return;
    }
}

void fg(int jobnum, jobs background[], jobs *fgdetails, int *Cstatus)
{
    int pid = -1, status;
    for (int i = 0; i < 20; i++)
    {
        if (background[i].job_id == jobnum)
        {
            pid = background[i].pid;
            fgdetails->pid = pid;
            strcpy(fgdetails->name, background[i].name);
            background[i].job_id = -1;
            strcpy(background[i].name, "\0");
            background[i].pid = -1;
            break;
        }
    }
    if (pid == -1)
    {
        printf("Process doesn't exists in the background\n");
        *Cstatus = 1;
        return;
    }

    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    tcsetpgrp(0, getpgid(pid));
    if (kill(pid, SIGCONT) == -1)
    {
        *Cstatus = 1;
        printf("error\n");
        return;
    }
    waitpid(pid, &status, WUNTRACED);
    tcsetpgrp(0, getpgid(getpid()));
    signal(SIGTTOU, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
    if (WIFSTOPPED(status) || WEXITSTATUS(status))
        *Cstatus = 1;
    if (WIFSTOPPED(status))
    {
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
}