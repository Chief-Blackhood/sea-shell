#include "headers.h"
#include "echo.h"

void echo(char **argv)
{
    int i = 1;
    while (argv[i] != NULL)
    {
        printf("%s ", argv[i++]);
    }
    printf("\n");
}