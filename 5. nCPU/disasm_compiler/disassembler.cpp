#include "disassembler.h"

int main()
{
    FILE *source      = fopen("compiled_code.txt", "rb");
    FILE *disasm_list = fopen("disasm_code_listing.txt", "wb");
    FILE *res         = fopen("disasm_code.txt", "wb");

    assert(source);
    assert(res);

    disasm_code(source, res, disasm_list);

    fclose(source);
    fclose(res);
    fclose(disasm_list);

    return 0;
}
