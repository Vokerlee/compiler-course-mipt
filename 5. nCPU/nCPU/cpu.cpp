#include "cpu.h"
#include <TXlib.h>

int main()
{
    system("asm_compiler.exe");

    FILE *source = fopen("compiled_code.txt", "rb");
    FILE *res    = fopen("output.txt", "wb");

    assert(source);
    assert(res);

    CPU cpu = {};

    cpu_handler(&cpu, source, res);

    fclose(source);
    fclose(res);

    return 0;
}
