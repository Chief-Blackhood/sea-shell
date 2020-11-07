#include "headers.h"
#include "pwd.h"

void pwd()
{
    char buf[1024];
    printf("%s\n", getcwd(buf, 1024));
}