#include "hostname.h"
#include "headers.h"

void hostname(int *Cstatus)
{
    char hostname[1023];
    struct passwd *p = getpwuid(getuid());
    if (p == NULL)
    {
        perror("getpwuid");
        return;
    }
    if (gethostname(hostname, 1023) == -1)
    {
        perror("gethostname");
        return;
    }

    if ((*Cstatus) == 0)
    {
        printf("\033[1;32m");
        printf(":')");
    }
    else if ((*Cstatus) == 1)
    {
        printf("\033[1;35m");
        printf(":'(");
    }
    printf("\033[0m");
    printf("<");
    printf("\033[1;32m");
    printf("%s@%s", p->pw_name, hostname);
    printf("\033[0m");
    printf(":");
}