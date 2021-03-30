#include "language_parsing.h"

int main()
{
    printf("Please, enter the file-name you want to translate into the ancient_ruses language:\n");

    char *file_name = (char *) calloc(100, sizeof(char));

    scanf("%s", file_name);

    FILE *formula = fopen(file_name, "rb");
    assert(formula);

    tree_to_lang(formula);

    fclose(formula);
    free(file_name);

    printf("Everything is OK. Your file is lang.txt\n");

    system("pause");
    return 0;
}
