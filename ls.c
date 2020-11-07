#include "headers.h"
#include "ls.h"

void permissions(char *per, mode_t bits)
{
    per[0] = (bits & S_IFDIR) == 0 ? '-' : 'd';
    per[1] = (bits & S_IRUSR) == 0 ? '-' : 'r';
    per[2] = (bits & S_IWUSR) == 0 ? '-' : 'w';
    per[3] = (bits & S_IXUSR) == 0 ? '-' : 'x';
    per[4] = (bits & S_IRGRP) == 0 ? '-' : 'r';
    per[5] = (bits & S_IWGRP) == 0 ? '-' : 'w';
    per[6] = (bits & S_IXGRP) == 0 ? '-' : 'x';
    per[7] = (bits & S_IROTH) == 0 ? '-' : 'r';
    per[8] = (bits & S_IWOTH) == 0 ? '-' : 'w';
    per[9] = (bits & S_IXOTH) == 0 ? '-' : 'x';
}

void ls(char **argv, int argc, int *Cstatus)
{
    DIR *dir;
    struct dirent *dirStream;
    struct tm *time;
    struct stat info;
    char fla = 0, fl = 0, fa = 0, fcolon = 1, *values[10];
    char buf[1024], tempbuf[1024];
    int j = 0;
    long long total = 0, totalwithhidden = 0;
    struct passwd *user;
    struct group *grp;
    char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "July", "Aug", "Sep", "Oct", "Nov", "Dec"};

    for (int i = 0; i < 10; i++)
        values[i] = NULL;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i] != NULL)
        {
            if (argv[i][0] == '-')
            {
                if (strcmp(argv[i], "-la") == 0 || strcmp(argv[i], "-al") == 0)
                {
                    fla = 1;
                }
                else if (strcmp(argv[i], "-a") == 0)
                    fa = 1;
                else if (strcmp(argv[i], "-l") == 0)
                    fl = 1;
            }
            else
            {
                values[j++] = argv[i];
            }
        }
    }

    if (values[0] == NULL)
        values[0] = ".";

    if (values[1] == NULL)
        fcolon = 0;

    for (int i = 0; values[i] != NULL; i++)
    {

        if (*values[i] == '~')
        {
            strcpy(tempbuf, getenv("PWD"));
            int k = 0, l = strlen(values[i]), bufLength = strlen(getenv("PWD"));

            for (int j = 1; j < l; j++)
            {
                tempbuf[bufLength + k] = values[i][j];
                k++;
            }
            values[i] = tempbuf;
        }
        if (access(values[i], F_OK) == -1)
        {
            *Cstatus = 1;
            printf("%s doesn't exists\n", values[i]);
        }
        else
        {
            stat(values[i], &info);
            if (S_ISDIR(info.st_mode) == 0)
                printf("%s is not a directory\n", values[i]);
            else
            {
                if (fcolon)
                    printf("\n%s:\n", values[i]);
                dir = opendir(values[i]);
                while ((dirStream = readdir(dir)) != NULL)
                {
                    sprintf(buf, "%s/%s", values[i], dirStream->d_name);
                    stat(buf, &info);
                    if (*(dirStream->d_name) != '.')
                        total += info.st_blocks;
                    totalwithhidden += info.st_blocks;
                }
                closedir(dir);
                dir = opendir(values[i]);
                if (fla == 1 || (fa == 1 && fl == 1))
                {
                    printf("total %lld\n", totalwithhidden / 2);
                    while ((dirStream = readdir(dir)) != NULL)
                    {
                        char per[11];
                        sprintf(buf, "%s/%s", values[i], dirStream->d_name);
                        stat(buf, &info);
                        time = localtime(&(info.st_mtime));
                        permissions(per, info.st_mode);
                        per[10] = '\0';
                        user = getpwuid(info.st_uid);
                        grp = getgrgid(info.st_gid);
                        printf("%s\t%ld\t%s\t%s\t%ld\t%s\t%2d\t%02d:%02d\t%s\n", per, info.st_nlink, user->pw_name,
                               grp->gr_name, info.st_size, months[time->tm_mon],
                               time->tm_mday, time->tm_hour, time->tm_min, dirStream->d_name);
                    }
                }
                else if (fa == 1)
                {
                    while ((dirStream = readdir(dir)) != NULL)
                    {
                        printf("%s\n", dirStream->d_name);
                    }
                }
                else if (fl == 1)
                {
                    printf("total %lld\n", total / 2);
                    while ((dirStream = readdir(dir)) != NULL)
                    {
                        if (*(dirStream->d_name) != '.')
                        {
                            char per[11];
                            sprintf(buf, "%s/%s", values[i], dirStream->d_name);
                            stat(buf, &info);
                            time = localtime(&(info.st_mtime));
                            permissions(per, info.st_mode);
                            per[10] = '\0';
                            user = getpwuid(info.st_uid);
                            grp = getgrgid(info.st_gid);
                            printf("%s\t%ld\t%s\t%s\t%ld\t%s\t%2d\t%02d:%02d\t%s\n", per, info.st_nlink, user->pw_name,
                                   grp->gr_name, info.st_size, months[time->tm_mon],
                                   time->tm_mday, time->tm_hour, time->tm_min, dirStream->d_name);
                        }
                    }
                }
                else
                {
                    while ((dirStream = readdir(dir)) != NULL)
                    {
                        if (*(dirStream->d_name) != '.')
                            printf("%s\n", dirStream->d_name);
                    }
                }
                closedir(dir);
            }
        }
    }
}