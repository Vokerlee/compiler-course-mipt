#include "text.h"

int main()
{
    FILE *code    = fopen("code.txt", "rb");
    FILE *results = fopen("res.txt", "wb");

    text_t text = {};

    construct_text(&text);

    fill_text(code, &text, "no comments");

    print_text_lines(results, &text);

    destruct_text(&text);

    fclose(code);
    fclose(results);
    system("pause");

    return 0;
}
