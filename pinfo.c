#include "headers.h"
#include "pinfo.h"

void pinfo(pid_t pid, int *Cstatus)
{
    char path[50], line[100];
    long long vm, fgpid, gpid;
    char *status;
    FILE *fd;

    printf("pid -- %d\n", pid);

    snprintf(path, 50, "/proc/%d/stat", pid);

    fd = fopen(path, "r");
    if (!fd)
    {
        *Cstatus = 1;
        perror("fopen:");
        return;
    }

    for (int i = 0; i < 23; i++)
    {
        if (i == 2)
        {
            fscanf(fd, "%s", line);
            status = (char *)malloc(20);
            strcpy(status, line);
        }
        else if (i == 4)
            fscanf(fd, "%lld", &gpid);
        else if (i == 7)
        {
            fscanf(fd, "%lld", &fgpid);
        }
        else if (i == 22)
        {
            fscanf(fd, "%lld", &vm);
        }
        else
        {
            fscanf(fd, "%s", line);
        }
    }
    fclose(fd);
    printf("Process Status -- %s", status);
    if (fgpid == gpid)
        printf("+\n");
    else
        printf("\n");
    printf("memory -- %lld\n", vm);

    snprintf(path, 50, "/proc/%d/exe", pid);
    ssize_t len = readlink(path, line, 100);
    line[len] = '\0';
    if (len != -1)
    {
        printf("Executable Path -- %s\n", line);
    }
    else
    {
        printf("Permission to read the file is denied\n");
    }
    return;
}