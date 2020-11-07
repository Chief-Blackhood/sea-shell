#include "headers.h"
#include "history.h"

void history(int num, int fshow, char *startDir, char *command)
{
    char *historylist[21];
    char *line;
    size_t len = 0;
    int index = 0, i = 0, fsame = 1;
    char *path;
    path = (char *)malloc(200);
    if (path == NULL)
    {
        perror("Unable to allocate buffer");
        return;
    }
    strcpy(path, startDir);
    strcat(path, "/history.txt");

    for (int i = 0; i < 21; i++)
        historylist[i] = NULL;

    line = (char *)malloc(len * sizeof(char));

    if (line == NULL)
    {
        perror("Unable to allocate buffer");
        return;
    }
    FILE *fp = fopen(path, "a");
    if (!fp)
    {
        printf("Creation of history file failed\n");
        return;
    }
    fclose(fp);
    fp = fopen(path, "r");
    if (fp)
    {
        while (getline(&line, &len, fp) != -1)
        {
            historylist[index] = (char *)malloc(100 * sizeof(char));
            strcpy(historylist[index], line);
            index++;
        }
        free(line);
    }
    else
    {
        printf("Unable to open history.txt file\n");
        return;
    }
    fclose(fp);
    index = 0;
    for (int i = 0; i < 21; i++)
    {
        if (historylist[i] != NULL)
            index++;
        else
            break;
    }
    if (fshow == 0)
    {
        if (index == 0)
        {
            historylist[0] = (char *)malloc(100 * sizeof(char));
            if (historylist[0] == NULL)
            {
                perror("Unable to allocate buffer");
                return;
            }
            strcpy(historylist[0], command);
        }
        else
        {
            if (strcmp(historylist[index - 1], command) != 0)
                fsame = 0;
        }
        fp = fopen(path, "w");
        if (fp)
        {
            if (fsame)
            {
                for (int i = 0; historylist[i] != NULL; i++)
                    fprintf(fp, "%s", historylist[i]);
            }
            else if (index >= 20)
            {
                for (int i = 1; historylist[i] != NULL; i++)
                    fprintf(fp, "%s", historylist[i]);
                fprintf(fp, "%s", command);
            }
            else
            {
                for (int i = 0; historylist[i] != NULL; i++)
                    fprintf(fp, "%s", historylist[i]);
                fprintf(fp, "%s", command);
            }
        }
        fclose(fp);
    }
    else
    {
        if (num > 20)
        {
            num = 20;
            printf("Only 20 commands should be displayed. Thus displaying those 20 commands\n");
        }
        int start = index - num < 0 ? 0 : index - num;
        for (int i = start; i < index; i++)
            printf("%s", historylist[i]);
    }
    free(path);
    for (int i = 0; i < 21; i++)
        free(historylist[i]);
}