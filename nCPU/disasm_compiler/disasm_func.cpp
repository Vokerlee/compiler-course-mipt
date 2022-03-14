#include "disassembler.h"

void disasm_code (FILE *source, FILE *res, FILE *disasm_list)
{
    assert(source);
    assert(res);
    assert(disasm_list);

    id_verification(source);

    print_disasm_list_header(disasm_list);

    int bytes = 0;
    fread(&bytes, sizeof(int), 1, source);

    char *code = (char *) calloc(bytes, sizeof(char));
    fread(code, sizeof(char), bytes, source);

    int n_labels = 0;
    fread(&n_labels, sizeof(int), 1, source);

    int *labels = nullptr;

    if (n_labels != 0)
    {
        labels = (int *) calloc(n_labels, sizeof(int));
        fread(labels, sizeof(int), n_labels, source);
    }

    disasm_command(bytes, code, labels, res, disasm_list);

    free(code);
    free(labels);
}

void print_disasm_list_header (FILE *res)
{
    assert(res);

    fprintf(res, "Disassembler:\n"
                 "compiled_code %*.c ->", FILE_LISTING_INDENT / 2 - 7, ' ');

    fprintf(res, "%*.c code\n", FILE_LISTING_INDENT / 2 + FILE_LISTING_INDENT % 2 - 7, ' ');
}

void disasm_command (int bytes, char *code, int *labels, FILE *res, FILE *disasm_list)
{
    assert(code);
    assert(res);
    assert(disasm_list);

    int counter_bytes  = 0;
    double number      = 0;
    int index_ram      = 0;
    int label_counter  = 0;

    while (counter_bytes < bytes)
    {
        unsigned char command = *(char *)(code + counter_bytes);

        if (labels != nullptr && counter_bytes == labels[label_counter])
        {
            int alignment_var = 0;

            fprintf(disasm_list, "\"");
            hex_print((unsigned char *) &(labels[label_counter]), sizeof(int), disasm_list);
            fprintf(disasm_list, "\"%*.c", FILE_LISTING_INDENT - 9, ' ');

            fprintf(res, "label%d:\n", label_counter + 1);
            fprintf(disasm_list, "label%d:\n", label_counter + 1);

            label_counter++;

            continue;
        }

        counter_bytes += sizeof(char);

        fprintf(disasm_list, "\"");
        hex_print((unsigned char *) &command, sizeof(char), disasm_list);

        if (command > 100)
        {

            if (command % 100 >= 1 && command % 100 <= 9)
            {
                fprintf(disasm_list, "\"%*.c", FILE_LISTING_INDENT, ' ');
                fprintf(disasm_list, "push r%cx\n", 'a' + command % 10 - 1);
                fprintf(res, "push r%cx\n", 'a' + command % 10 - 1);

                continue;
            }

            if (command % 110 >= 1 && command % 110 <= 9)
            {
                fprintf(disasm_list, "\"%*.c", FILE_LISTING_INDENT, ' ');
                fprintf(disasm_list, "pop r%cx\n", 'a' + command % 10 - 1);
                fprintf(res, "pop r%cx\n", 'a' + command % 10 - 1);
                continue;
            }

            if (command % 120 >= 1 && command % 120 <= 9)
            {
                fprintf(disasm_list, "\"%*.c", FILE_LISTING_INDENT, ' ');
                fprintf(disasm_list, "in r%cx\n", 'a' + command % 10 - 1);
                fprintf(res, "in r%cx\n", 'a' + command % 10 - 1);
                continue;
            }

            if (command == 130 || (command % 130 >= 1 && command % 130 <= 9))
            {
                fprintf(disasm_list, "\"%*.c", FILE_LISTING_INDENT, ' ');

                index_ram = *(int *)(code + counter_bytes);

                if (command == 130)
                {
                    fprintf(disasm_list, "push [%d]\n", index_ram);
                    fprintf(res, "push [%d]\n", index_ram);
                }
                else
                {
                    if (index_ram > 0)
                    {
                        fprintf(disasm_list, "push [r%cx + %d]\n", 'a' + command % 10 - 1, index_ram);
                        fprintf(res, "push [r%cx + %d]\n", 'a' + command % 10 - 1, index_ram);
                    }
                    else
                    {
                        fprintf(disasm_list, "push [r%cx - %d]\n", 'a' + command % 10 - 1, -index_ram);
                        fprintf(res, "push [r%cx - %d]\n", 'a' + command % 10 - 1, -index_ram);
                    }
                }

                counter_bytes += sizeof(int);

                continue;
            }

            if (command == 140 || (command % 140 >= 1 && command % 140 <= 9))
            {
                fprintf(disasm_list, "\"%*.c", FILE_LISTING_INDENT, ' ');

                index_ram = *(int *)(code + counter_bytes);

                if (command == 140)
                {
                    fprintf(disasm_list, "pop [%d]\n", index_ram);
                    fprintf(res, "pop [%d]\n", index_ram);
                }
                else
                {
                    if (index_ram > 0)
                    {
                        fprintf(disasm_list, "pop [r%cx + %d]\n", 'a' + command % 10 - 1, index_ram);
                        fprintf(res, "pop [r%cx + %d]\n", 'a' + command % 10 - 1, index_ram);
                    }
                    else
                    {
                        fprintf(disasm_list, "pop [r%cx - %d]\n", 'a' + command % 10 - 1, -index_ram);
                        fprintf(res, "pop [r%cx - %d]\n", 'a' + command % 10 - 1, -index_ram);
                    }
                }

                counter_bytes += sizeof(int);

                continue;
            }

            if (command == 150 || (command % 150 >= 1 && command % 150 <= 9))
            {
                fprintf(disasm_list, "\"%*.c", FILE_LISTING_INDENT, ' ');

                index_ram = *(int*)(code + counter_bytes);

                if (command == 150)
                {
                    fprintf(disasm_list, "in [%d]\n", index_ram);
                    fprintf(res, "in [%d]\n", index_ram);
                }
                else
                {
                    if (index_ram > 0)
                    {
                        fprintf(disasm_list, "in [r%cx + %d]\n", 'a' + command % 10 - 1, index_ram);
                        fprintf(res, "in [r%cx + %d]\n", 'a' + command % 10 - 1, index_ram);
                    }
                    else
                    {
                        fprintf(disasm_list, "in [r%cx - %d]\n", 'a' + command % 10 - 1, -index_ram);
                        fprintf(res, "in [r%cx - %d]\n", 'a' + command % 10 - 1, -index_ram);
                    }
                }

                counter_bytes += sizeof(int);

                continue;
            }
        }

        switch (command)
        {
            #define DEF_CMD(name, num, hash, code_cpu, code_disasm)     \
                case name: code_disasm;                                 \
                break;

            #include "commands.h"

            #undef DEF_CMD
        }

        fprintf(disasm_list, "\n");
        fprintf(res, "\n");
    }
}

void id_verification (FILE *source)
{
    assert(source);

    char ID[4] = {0};

    fread(&ID, sizeof(char), 3, source);

    if (strcmp(ID, "ID6") != 0)
    {
        FILE *log_disasm = fopen("log_disasm.txt", "ab");
        fprintf(log_disasm, "ID OF COMPILED CODE BY ASSEMBLER DOESN'T COINCIDES %s\n", ID);
        fclose(log_disasm);
    }
}

void hex_print (unsigned char *hex, size_t size_hex, FILE *res)
{
    assert(hex);
    assert(res);

    for (int i = size_hex - 1; i >= 1; i--)
        fprintf(res, "%02x ", hex[i]);

    fprintf(res, "%02x", hex[0]);
}
