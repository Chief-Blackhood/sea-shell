#include "parse.h"
#include "headers.h"

void parse(char *command, char **argv)
{
    char *part;
    char *temp;
    temp = malloc(100);
    if (temp == NULL)
    {
        printf("Unable to allocate memory");
        return;
    }
    temp[0] = '\0';
    strcpy(temp, command);
    part = strtok(temp, " ");
    int i = 0;
    while (part != NULL)
    {
        argv[i] = (char *)malloc(100);
        strcpy(argv[i], part);
        i++;
        part = strtok(NULL, " ");
    }
    argv[i] = NULL;
    free(temp);
}