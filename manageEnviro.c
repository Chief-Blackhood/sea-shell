#include "headers.h"
#include "manageEnviro.h"

void manageenviro(char **argv, int option, int *Cstatus)
{
    switch (option)
    {
    case 0:
        if ((argv[1] == NULL) && argv[2] == NULL)
        {
            *Cstatus = 1;
            printf("Not enough input arguments\n");
            return;
        }
        if (argv[2] != NULL && argv[3] != NULL)
        {
            *Cstatus = 1;
            printf("Too many input arguments\n");
            return;
        }
        if (argv[2] == NULL)
        {
            if (setenv(argv[1], "", 1) < 0)
            {
                *Cstatus = 1;
                return;
            }
        }
        else
        {
            if (setenv(argv[1], argv[2], 1) < 0)
            {
                *Cstatus = 1;
                return;
            }
        }

        break;
    case 1:
        if (argv[1] == NULL)
        {
            *Cstatus = 1;
            printf("Not enough input arguments\n");
            return;
        }
        if (argv[2] != NULL)
        {
            *Cstatus = 1;
            printf("Too many input arguments\n");
            return;
        }
        if (getenv(argv[1]) != NULL)
        {
            if (unsetenv(argv[1]) < 0)
            {
                *Cstatus = 1;
                return;
            }
            printf("The variable has been erased\n");
        }
        else
        {
            printf("The variable does not exist\n");
        }
        break;
    default:
        break;
    }
}