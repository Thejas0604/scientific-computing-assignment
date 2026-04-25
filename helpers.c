#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int findusers()
{
    char tmp[1024];
    char *line, *record;

    FILE *fstream = fopen("/mnt/e/coding-projects/scientific-computing-assignment/Dataset/ratings_learn.csv", "r");

    if (!fstream) {
        printf("Error opening file\n");
        return 0;
    }

    int max = 0;

    while ((line = fgets(tmp, sizeof(tmp), fstream)) != NULL)
    {
        record = strtok(line, ",");

        if (record != NULL) {
            int t = atoi(record);
            if (t > max)
                max = t;
        }
    }

    fclose(fstream);
    return max;
}