#include "disassembler.h"

int main(int argc, char *argv[])
{
    FILE *executable  = fopen(argv[1], "rb");
    FILE* disasm      = fopen(argv[2], "wb");

    const char* disasm_listing_name = nullptr;
    if (!argv[2])
        disasm_listing_name = "disasm_code_listing.txt.txt";
    else
        disasm_listing_name = argv[3];

    FILE *disasm_listing = fopen(disasm_listing_name, "wb");

    if (!executable)
    {
        printf("\nSorry, file \"%s\" doesn't exist\n", argv[1]);

        if (disasm)
            fclose(disasm);

        if (disasm_listing)
            fclose(disasm_listing);
    }
    else if (!disasm)
    {
        printf("\nSorry, file \"%s\" cannot be opened (system error)\n", argv[1]);

        if (executable)
            fclose(executable);

        if (disasm_listing)
            fclose(disasm_listing);
    }
    else
    {
        disasm_code(executable, disasm, disasm_listing);

        fclose(executable);
        fclose(disasm);
        fclose(disasm_listing);
    }

    return 0;
}