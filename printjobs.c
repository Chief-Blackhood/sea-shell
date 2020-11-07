#include "headers.h"
#include "jobs.h"
#include "printjobs.h"

int comparator(const void *p, const void *q)
{
    jobs *l = (jobs *)p;
    jobs *m = (jobs *)q;

    int a = l->job_id;
    int b = m->job_id;

    if (a == -1)
        return 1;
    if (b == -1)
        return -1;
    return a - b;
}

void printjobs(jobs background[])
{
    char path[50], line[100];
    char *status;
    FILE *fd;
    qsort(background, 20, sizeof(background[0]), comparator);
    for (int i = 0; i < 20; i++)
    {
        if (background[i].job_id != -1)
        {
            snprintf(path, 50, "/proc/%d/stat", background[i].pid);

            fd = fopen(path, "r");
            if (!fd)
            {
                printf("No background process running\n");
                return;
            }

            for (int i = 0; i < 23; i++)
            {
                if (i == 2)
                {
                    fscanf(fd, "%s", line);
                    status = (char *)malloc(10);
                    strcpy(status, line);
                    break;
                }
                else
                {
                    fscanf(fd, "%s", line);
                }
            }
            printf("[%d] %s %s [%d]\n", background[i].job_id, (strcmp(status, "T") == 0) ? "Stopped" : "Running", background[i].name, background[i].pid);
        }
    }
}

void kjob(jobs background[], int jobnum, int sig, int *Cstatus)
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
        printf("No such job present in the background\n");
        return;
    }
    if (kill(pid, sig) < 0)
    {
        *Cstatus = 1;
        perror("error occured in sending signal");
        return;
    }
}