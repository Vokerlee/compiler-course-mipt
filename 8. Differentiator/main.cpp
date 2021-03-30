#include "diff.h"

int main()
{
    FILE *formula = fopen("formula.txt", "rb");
    assert(formula);

    create_diff_article(formula);

    fclose(formula);

    system("pause");

    return 0;
}
