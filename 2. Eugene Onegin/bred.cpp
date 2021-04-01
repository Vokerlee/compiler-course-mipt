#include "sort_header.h"

void bred_generator (line_t *lines, const char *bred_ind, int n)
{
    assert(lines);
    assert(bred_ind);

    FILE *res = fopen("bred.txt", "w");

    if (strcmp(bred_ind, "bred+"))
        return;

    int random1 = 0;
    int random2 = 0;

    srand(time(NULL));

    for (size_t i = 0; i < BRED; i++)
    {
        if (rand() % 2 == 1)
        {
            random1 = rand() % (n - 1);
            for (size_t j = 0; j < lines[random1].length; j++)
                fprintf (res, "%c", lines[random1].line[j]);

            random2 = rand() % (n - 1);
            for (size_t j = 0; j < lines[random2].length; j++)
                fprintf (res, "%c", lines[random2].line[j]);

            random1++;
            random2++;

            for (size_t j = 0; j < lines[random1].length; j++)
                fprintf (res, "%c", lines[random1].line[j]);
            for (size_t j = 0; j < lines[random2].length; j++)
                fprintf (res, "%c", lines[random2].line[j]);

            fprintf (res, "\n");
        }
        else
        {
            random1 = rand() % (n - 1);
            for (size_t j = 0; j < lines[random1].length; j++)
                fprintf (res, "%c", lines[random1].line[j]);

            random1++;

            for (size_t j = 0; j < lines[random1].length; j++)
                fprintf (res, "%c", lines[random1].line[j]);

            random2 = rand() % (n - 1);
            for (size_t j = 0; j < lines[random2].length; j++)
                fprintf (res, "%c", lines[random2].line[j]);

            random2++;

            for (size_t j = 0; j < lines[random2].length; j++)
                fprintf (res, "%c", lines[random2].line[j]);

            fprintf (res, "\n");
        }
    }
}
