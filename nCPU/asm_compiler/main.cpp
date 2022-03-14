#include "compile.h"

int main(int argc, char *argv[])
{
    FILE *asm_code   = fopen(argv[1], "rb");
    FILE *executable = fopen(argv[2], "wb");

    if (!asm_code && !executable)
        printf("\nSorry, file \"%s\" doesn't exist and file \"%s\" cannot be open (system error)\n", argv[1], argv[2]);
    else if (!asm_code)
    {
        printf("\nSorry, file \"%s\" doesn't exist\n", argv[1]);
        fclose(executable);
    }
    else if (!executable)
    {
        printf("\nSorry, file \"%s\" cannot be opened (system error).\n", argv[2]);
        fclose(asm_code);
    }
    else
    {
        text_t text = {};
        construct_text(&text);

        fill_text(asm_code, &text, NO_COMMENTS);

        compile(&text, executable);

        destruct_text(&text);

        fclose(asm_code);
        fclose(executable);

        printf("OK. The assembler program is compiled into executable .nasm file.\n");
    }

    return 0;
}
