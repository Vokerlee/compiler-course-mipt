#include "compile.h"

int main()
{
    FILE *source = fopen("code.txt", "rb");
    FILE *res    = fopen("compiled_code.txt", "wb");

    assert(source);
    assert(res);

    text_t text = {};
    construct_text(&text);

    fill_text(source, &text, NO_COMMENTS);

    compile(&text, res);

    destruct_text(&text);

    fclose(source);
    fclose(res);

    return 0;
}
