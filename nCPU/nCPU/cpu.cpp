#include "cpu.h"

void cpu_handler (CPU *cpu, FILE *source, FILE *res)
{
    assert(source);
    assert(res);
    assert(cpu);

    id_verification(source);

    cpu->bytes_cmd = 0;
    fread(&(cpu->bytes_cmd), sizeof(int), 1, source);

    cpu->code = (char *) calloc(cpu->bytes_cmd, sizeof(char));
    fread(cpu->code, sizeof(char), cpu->bytes_cmd, source);

    cpu->n_labels = 0;
    fread(&(cpu->n_labels), sizeof(int), 1, source);

    if (cpu->n_labels != 0)
    {
        cpu->labels = (int *) calloc(cpu->n_labels, sizeof(int));
        fread(cpu->labels, sizeof(int), cpu->n_labels, source);
    }

    construct_stack(&(cpu->stk),      STACK_INIT_MAXCOUNT, "cpu_stk");
    construct_stack(&(cpu->stk_call), STACK_INIT_MAXCOUNT, "cpu_stk_call");

    commands_handler(cpu, res);

    fprintf(res, "Remaining numbers in stack are: \n");

    for (int i = 0; i < cpu->stk.counter; i++)
        fprintf(res, "%lg\n", cpu->stk.data[i]);

    destruct_stack(&(cpu->stk));
    destruct_stack(&(cpu->stk_call));
    free(cpu->code);
    free(cpu->labels);
}

void commands_handler (CPU *cpu, FILE *res)
{
    assert(cpu);
    assert(res);

    size_t counter_bytes  = 0;
    double number         = 0;
    double temp_1         = 0;
    double temp_2         = 0;
    int index_ram         = 0;
    int label             = 0;

    while (counter_bytes < cpu->bytes_cmd)
    {
        unsigned char command = *(unsigned char *)(cpu->code + counter_bytes);

        counter_bytes += sizeof(char);

        if (command > 100)
        {
            if (command % 100 >= 1 && command % 100 <= 9)
            {
                push_stack(&(cpu->stk), cpu->rix[command % 10 - 1]);
                continue;
            }

            if (command % 110 >= 1 && command % 110 <= 9)
            {
                cpu->rix[command % 10 - 1] = pop_stack(&(cpu->stk));
                continue;
            }

            if (command % 120 >= 1 && command % 120 <= 9)
            {
                scanf("%lf", &(cpu->rix[command % 10 - 1]));
                continue;
            }

            if (command == 130 || (command % 130 >= 1 && command % 130 <= 9))
            {
                index_ram = *(int*)(cpu->code + counter_bytes);

                if (command == 130)
                    push_stack(&(cpu->stk), cpu->RAM[index_ram]);
                else
                    push_stack(&(cpu->stk), cpu->RAM[(int) cpu->rix[command % 10 - 1] + index_ram]);

                counter_bytes += sizeof(int);

                continue;
            }

            if (command == 140 || (command % 140 >= 1 && command % 140 <= 9))
            {
                index_ram = *(int*)(cpu->code + counter_bytes);

                if (command == 140)
                    cpu->RAM[index_ram] = pop_stack(&(cpu->stk));
                else
                    cpu->RAM[(int) cpu->rix[command % 10 - 1] + index_ram] = pop_stack(&(cpu->stk));

                counter_bytes += sizeof(int);

                continue;
            }

            if (command == 150 || (command % 150 >= 1 && command % 150 <= 9))
            {
                index_ram = *(int*)(cpu->code + counter_bytes);

                if (command == 150)
                    scanf("%lf", &(cpu->RAM[index_ram]));
                else
                    scanf("%lf", &(cpu->RAM[(int) cpu->rix[command % 10 - 1] + index_ram]));

                counter_bytes += sizeof(int);

                continue;
            }
        }

        switch (command)
        {
            #define DEF_CMD(name, num, hash, code_cpu, code_disasm)     \
                case num: code_cpu;                                     \
                break;

            #include "commands.h"

            #undef DEF_CMD
        }
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

