#include "sort_header.h"

void exch (line_t *lines, int l, int r)
{
    assert(lines);

    char *strtmp = lines[l].line;
    lines[l].line = lines[r].line;
    lines[r].line = strtmp;

    int tmp = lines[l].length;
    lines[l].length = lines[r].length;
    lines[r].length = tmp;
}

int strcomp_ascen (const void *line1, const void *line2)
{
    assert(line1);
    assert(line2);

    int i = 0;
    int j = 0;

    while (!isalpha(((struct line_t*) line1)->line[i]) || ((struct line_t*) line1)->line[i] == '\'')
        i++;
    while (!isalpha(((struct line_t*) line2)->line[j]) || ((struct line_t*) line2)->line[j] == '\'')
        j++;

    while (i < ((struct line_t*) line1)->length && j < ((struct line_t*) line2)->length)
    {
        if (((struct line_t*) line1)->line[i] == ((struct line_t*) line2)->line[j])
        {
            i++;
            j++;
        }
        else
            return (((struct line_t*) line1)->line[i] - ((struct line_t*) line2)->line[j]);
    }

    return (((struct line_t*) line1)->line[i] - ((struct line_t*) line2)->line[j]);
}

int strcomp_descen (const void *line1, const void *line2)
{
    assert(line1);
    assert(line2);

    return -(strcomp_ascen(line1 , line2));
}

int strcomp_rhyme (const void *line1, const void *line2)
{
    assert(line1);
    assert(line2);

    int len1 = ((struct line_t*) line1)->length;
    int len2 = ((struct line_t*) line2)->length;

    while (!isalpha(((struct line_t*) line1)->line[len1-1]))
        len1--;
    while (!isalpha(((struct line_t*) line2)->line[len2-1]))
        len2--;

    int len10 = len1;
    int len20 = len2;

    len1 -= RHYME;
    len2 -= RHYME;

    while (len1 <= len10 && len2 <= len20)
    {
        if (((struct line_t*) line1)->line[len10 - 1] == ((struct line_t*) line2)->line[len20 - 1])
        {
            len10--;
            len20--;
        }
        else
            return (((struct line_t*) line1)->line[len10 - 1] - ((struct line_t*) line2)->line[len20 - 1]);
    }

    return (((struct line_t*) line1)->line[len10 - 1] - ((struct line_t*) line2)->line[len20 - 1]);
}

void quicksort (line_t *lines, int l_0, int r_0, int (*str_comp)(const void*, const void*))
{
    assert(lines);
    assert(str_comp);

    if (r_0 - l_0 <= LIM_QSORT)
        return;

    int piv = (l_0 + r_0) / 2;
    int l = l_0;
    int r = r_0;

    do
    {
        while (str_comp((const void *) &lines[piv], (const void *) &lines[l]) > 0)
            l++;
        while (str_comp((const void *) &lines[r], (const void *) &lines[piv]) > 0)
            r--;

        if (l <= r)
            exch(lines, l++, r--);

    } while (l <= r);

    if (l_0 < r)
        quicksort(lines, l_0, r, str_comp);
    if (r_0 > l)
        quicksort(lines, l, r_0, str_comp);
}

void insertion (line_t *lines, int n, int (*str_comp)(const void*, const void*))
{
    assert(lines);
    assert(str_comp);

    for (size_t i = 0; i < n - 1; i++)
        for (size_t j = i + 1; j < n; j++)
	        if (str_comp((const void *) &lines[i], (const void *) &lines[j]) > 0)
                exch(lines, i, j);
}

void megasort (line_t *lines, int n, int (*str_comp)(const void*, const void*))
{
    assert(lines);
    assert(str_comp);

    quicksort(lines, 0, n - 1, str_comp);
    insertion(lines, n, str_comp);
}
