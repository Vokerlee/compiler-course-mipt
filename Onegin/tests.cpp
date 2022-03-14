#include "sort_header.h"

int comptexts (FILE *res_prog, FILE *res_test)
{
    assert(res_prog);
    assert(res_test);

    size_t n_alloc1 = 0;
    size_t n_alloc2 = 0;

    char **stroka_prog = (char **) calloc(MEMORY, sizeof(char*));
    char **stroka_test = (char **) calloc(MEMORY, sizeof(char*));

    assert(stroka_test);
    assert(stroka_prog);

    for (size_t j = 0; j < MEMORY; ++j)
    {
        stroka_prog[j] = (char *) calloc(LIM_LINE, sizeof(char));
        stroka_test[j] = (char *) calloc(LIM_LINE, sizeof(char));

        assert(stroka_test[j]);
        assert(stroka_prog[j]);
    }

    for (size_t j = 0; j < MEMORY; ++j)
    {
        if (stroka_prog[j])
            n_alloc1++;
        if (stroka_test[j])
            n_alloc2++;
    }

    if (n_alloc1 != MEMORY)
        DEBUG_PRINT("ERROR, STROKA_PROG HASN'T BEEN ALLOCATED");
    if (n_alloc2 != MEMORY)
        DEBUG_PRINT("ERROR, STROKA_TEST HASN'T BEEN ALLOCATED");

    size_t n_lines1 = 0;
    size_t n_lines2 = 0;

    while (fgets(stroka_prog[n_lines1++], LIM_LINE, res_prog) && n_lines1 < MEMORY);
    while (fgets(stroka_test[n_lines2++], LIM_LINE, res_test) && n_lines2 < MEMORY);

    n_lines1--;
    n_lines2--;

    if (n_lines1 == n_lines2)
    {
        for (size_t i = 0; i < n_lines1; ++i)
        {
            if (strcmp(stroka_test[i], stroka_prog[i]) != 0)
                return 0;
        }

        for (size_t j = 0; j < MEMORY; ++j)
        {
            free(stroka_prog[j]);
            free(stroka_test[j]);
        }

        free(stroka_test);
        free(stroka_prog);

        return 1;
    }
    else
        return 0;
}

#define TEST_SORTL(i)                                   \
    res_prog = fopen("res_prog"#i".txt", "w+");         \
    res_test = fopen("res_test"#i".txt", "r");          \
                                                        \
    test = fopen("test"#i".txt", "r");                  \
    sortlines(test, res_prog, "ascending", "bred-");    \
                                                        \
    fseek(res_prog, 0, SEEK_SET);                       \
    fseek(res_test, 0, SEEK_SET);                       \
                                                        \
    if ((temp = comptexts(res_prog, res_test)) == 0)    \
        printf("TEST "#i" FAILED\n\n");                 \
    else                                                \
        printf("TEST "#i" SUCCESSFULLY DONE\n\n");      \
                                                        \
    fclose(res_prog);                                   \
    fclose(res_test);                                   \
    fclose(test);

void sortlines_test()
{
    FILE *test     = NULL;
    FILE *res_prog = NULL;
    FILE *res_test = NULL;

    int temp = 0;

    TEST_SORTL(1);
    TEST_SORTL(2);
    TEST_SORTL(3);
    TEST_SORTL(4);
    TEST_SORTL(5);
}
