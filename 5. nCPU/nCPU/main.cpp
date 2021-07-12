#include "cpu.h"

int main(int argc, char *argv[])
{
    FILE *executable = fopen(argv[1], "rb");

    const char* result_file_name = nullptr;
    if (!argv[2])
        result_file_name = "results.txt";
    else
        result_file_name = argv[2];

    FILE *results = fopen(result_file_name, "wb");

    if (!executable && !results)
        printf("\nSorry, file \"%s\" doesn't exist and file \"%s\" cannot be open (system error)\n", argv[1], argv[2]);
    else if (!executable)
    {
        printf("\nSorry, file \"%s\" doesn't exist\n", argv[1]);
        fclose(results);
    }
    else if (!results)
    {
        printf("\nSorry, file \"%s\" cannot be opened (system error).\n", argv[2]);
        fclose(executable);
    }
    else
    {
        CPU cpu = {};
        cpu_handler(&cpu, executable, results);

        fclose(executable);
        fclose(results);
    }

    return 0;
}
