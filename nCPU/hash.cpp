#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int hash_text (const char *line)
{
    int temp = 1;

    size_t length_of_line = strlen(line);

    for (size_t i = 1; i <= length_of_line; i++)
        temp += temp * i * i * line[i - 1];

    return temp;
}

int main()
{
    printf("%d\n", hash_text("jae"));
    return 0;
}
