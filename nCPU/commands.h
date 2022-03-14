#define DO_POP             pop_stack(&(cpu->stk))
#define DO_PUSH(code)      push_stack(&(cpu->stk), code)
#define DO_POP_CALL        pop_stack(&(cpu->stk_call))
#define DO_PUSH_CALL(code) push_stack(&(cpu->stk_call), code)

#define COND_JUMP(condition)                                   \
    temp_1 = DO_POP;                                           \
    temp_2 = DO_POP;                                           \
                                                               \
    if (temp_2 condition temp_1)                               \
        counter_bytes = cpu->labels[label - 1];                \

#define DISASM_LIST_PRINT(command)                              \
    fprintf(disasm_list, "\"%*.c", FILE_LISTING_INDENT, ' ');   \
    fprintf(disasm_list, #command);                             \
    fprintf(res, #command);

#define DISASM_LIST_PRINT_LABEL(command)                               \
    fprintf(disasm_list, "\"");                                        \
    fprintf(disasm_list, "%*.c", FILE_LISTING_INDENT , ' ');           \
    fprintf(disasm_list, #command);                                    \
    fprintf(res, #command);                                            \
    int label = *(int *)(code + counter_bytes);                        \
    fprintf(res, "%d", label);                                         \
    fprintf(disasm_list, "%d", label);


DEF_CMD(ADD, 1, 35407498,
{
    temp_1 = DO_POP + DO_POP;
    DO_PUSH(temp_1);
},
{
    DISASM_LIST_PRINT(add)
})

DEF_CMD(SUB, 2, 48038732,
{
    DO_PUSH(-(DO_POP - DO_POP));
},
{
    DISASM_LIST_PRINT(sub)
})

DEF_CMD(NEG, 3, 41718240,
{
    DO_PUSH(-DO_POP);
},
{
    DISASM_LIST_PRINT(neg)
})

DEF_CMD(MUL, 4, 50197070,
{
    DO_PUSH(DO_POP * DO_POP);
},
{
    DISASM_LIST_PRINT(mul)
})

DEF_CMD(DIV, 5, 45199823,
{
    temp_1 = DO_POP;
    temp_2 = DO_POP;

    DO_PUSH(temp_2 / temp_1);
},
{
    DISASM_LIST_PRINT(div)
})

DEF_CMD(SQRT, 6, 1432092364,
{
    DO_PUSH(sqrt(DO_POP));
},
{
    DISASM_LIST_PRINT(sqrt)
})

DEF_CMD(POW, 7, 53905520,
{
    temp_1 = DO_POP;
    temp_2 = DO_POP;
    DO_PUSH(pow(temp_2, temp_1));
},
{
    DISASM_LIST_PRINT(pow)
})

DEF_CMD(EXP, 8, 49503558,
{
    DO_PUSH(exp(DO_POP));
},
{
    DISASM_LIST_PRINT(exp)
})

DEF_CMD(LN, 9, 48069,
{
    DO_PUSH(log(DO_POP));
},
{
    DISASM_LIST_PRINT(ln)
})

DEF_CMD(LOG10, 10, -1330049376,
{
    DO_PUSH(log10(DO_POP));
},
{
    DISASM_LIST_PRINT(log10)
})

DEF_CMD(MOD, 11, 44103950,
{
    DO_PUSH(fmod(DO_POP, DO_POP));
},
{
    DISASM_LIST_PRINT(mod)
})

DEF_CMD(FLOOR, 12, -981534968,
{
    DO_PUSH(floor(DO_POP));
},
{
    DISASM_LIST_PRINT(floor)
})

DEF_CMD(CEIL, 13, 1818667560,
{
    DO_PUSH(ceil(DO_POP));
},
{
    DISASM_LIST_PRINT(ceil)
})

DEF_CMD(MIN, 14, 45893210,
{
    DO_PUSH(fmin(DO_POP, DO_POP));
},
{
    DISASM_LIST_PRINT(min)
})

DEF_CMD(MAX, 15, 46255990,
{
    DO_PUSH(fmax(DO_POP, DO_POP));
},
{
    DISASM_LIST_PRINT(max)
})

DEF_CMD(SIN, 16, 48396476,
{
    DO_PUSH(sin(DO_POP));
},
{
    DISASM_LIST_PRINT(sin)
})

DEF_CMD(COS, 17, 46102000,
{
    DO_PUSH(cos(DO_POP));
},
{
    DISASM_LIST_PRINT(cos)
})

DEF_CMD(TG, 18, 48321,
{
    DO_PUSH(tan(DO_POP));
},
{
    DISASM_LIST_PRINT(tg)
})

DEF_CMD(CTG, 19, 43152000,
{
    DO_PUSH(1 / tan(DO_POP));
},
{
    DISASM_LIST_PRINT(ctg)
})

DEF_CMD(ARCSIN, 20, -812644880,
{
    DO_PUSH(asin(DO_POP));
},
{
    DISASM_LIST_PRINT(arcsin)
})

DEF_CMD(ARCCOS, 21, 852939328,
{
    DO_PUSH(acos(DO_POP));
},
{
    DISASM_LIST_PRINT(arccos)
})

DEF_CMD(ARCTG, 22, 1575666560,
{
    DO_PUSH(atan(DO_POP));
},
{
    DISASM_LIST_PRINT(arctg)
})

DEF_CMD(ARCCTG, 23, 785803704,
{
    DO_PUSH(acos(-1.0) / 2 - atan(DO_POP));
},
{
    DISASM_LIST_PRINT(arcctg)
})

DEF_CMD(SH, 24, 48372,
{
    temp_1 = DO_POP;

    DO_PUSH((exp(temp_1) - exp(-temp_1)) / 2);
},
{
    DISASM_LIST_PRINT(sh)
})

DEF_CMD(CH, 25, 41700,
{
    temp_1 = DO_POP;

    DO_PUSH((exp(temp_1) + exp(-temp_1)) / 2);
},
{
    DISASM_LIST_PRINT(ch)
})

DEF_CMD(TH, 26, 48789,
{
    temp_1 = DO_POP;

    DO_PUSH((exp(temp_1) - exp(-temp_1)) / (exp(temp_1) + exp(-temp_1)));
},
{
    DISASM_LIST_PRINT(th)
})

DEF_CMD(CTH, 27, 43570500,
{
    temp_1 = DO_POP;

    DO_PUSH((exp(temp_1) + exp(-temp_1)) / (exp(temp_1) - exp(-temp_1)));
},
{
    DISASM_LIST_PRINT(cth)
})

DEF_CMD(DUMP, 28, 1799451270,
{
    dump_stack(&(cpu->stk), res);
    dump_stack(&(cpu->stk), stdout);
},
{
    DISASM_LIST_PRINT(dump)
})

DEF_CMD(DUMP_CALL, 29, 2027370848,
{
    dump_stack(&(cpu->stk_call), res);
    dump_stack(&(cpu->stk_call), stdout);
},
{
    DISASM_LIST_PRINT(dump_call)
})

DEF_CMD(OUT, 30, 54891760,
{
    fprintf(res,    "The number in the pick of stack is %lg\n\n", top_stack(&(cpu->stk)));
    fprintf(stdout, "The number in the pick of stack is %lg\n\n", top_stack(&(cpu->stk)));
},
{
    DISASM_LIST_PRINT(out)
})

DEF_CMD(HLT, 31, 47510925,
{
    return;
},
{
    DISASM_LIST_PRINT(hlt)
})

DEF_CMD(JUMP, 32, -1835800358,
{
    label = *(int *)(cpu->code + counter_bytes);

    counter_bytes = cpu->labels[label - 1];
},
{
    DISASM_LIST_PRINT_LABEL(jump :label)
    counter_bytes += sizeof(int);
})

DEF_CMD(JA, 33, 41623,
{
    label = *(int *)(cpu->code + counter_bytes);
    counter_bytes += sizeof(int);

    COND_JUMP(>)
},
{
    DISASM_LIST_PRINT_LABEL(ja :label)
    counter_bytes += sizeof(int);
})

DEF_CMD(JAE, 34, 37876930,
{
    label = *(int *)(cpu->code + counter_bytes);
    counter_bytes += sizeof(int);

    COND_JUMP(>=)
},
{
    DISASM_LIST_PRINT_LABEL(jae :label)
    counter_bytes += sizeof(int);
})

DEF_CMD(JB, 35, 42051,
{
    label = *(int *)(cpu->code + counter_bytes);
    counter_bytes += sizeof(int);

    COND_JUMP(<)
},
{
    DISASM_LIST_PRINT_LABEL(jb :label)
    counter_bytes += sizeof(int);
})

DEF_CMD(JBE, 36, 38266410,
{
    label = *(int *)(cpu->code + counter_bytes);
    counter_bytes += sizeof(int);

    COND_JUMP(<=)
},
{
    DISASM_LIST_PRINT_LABEL(jbe :label)
    counter_bytes += sizeof(int);
})

DEF_CMD(JE, 37, 43335,
{
    label = *(int *)(cpu->code + counter_bytes);
    counter_bytes += sizeof(int);

    COND_JUMP(==)
},
{
    DISASM_LIST_PRINT_LABEL(je :label)
    counter_bytes += sizeof(int);
})

DEF_CMD(JNE, 38, 42940170,
{
    label = *(int *)(cpu->code + counter_bytes);
    counter_bytes += sizeof(int);

    COND_JUMP(!=)
},
{
    DISASM_LIST_PRINT_LABEL(jne :label)
    counter_bytes += sizeof(int);
})

DEF_CMD(CALL, 39, 1017621860,
{
    DO_PUSH_CALL(counter_bytes + sizeof(int));

    label = *(int *)(cpu->code + counter_bytes);

    counter_bytes = cpu->labels[label - 1];
},
{
    DISASM_LIST_PRINT_LABEL(call :label)
    counter_bytes += sizeof(int);
})

DEF_CMD(RET, 40, 48670875,
{
    counter_bytes = DO_POP_CALL;
},
{
    DISASM_LIST_PRINT(ret)
})

DEF_CMD(PUSH, 100, 1222331964,
{
    number = *(double *)(cpu->code + counter_bytes);
    DO_PUSH(number);
    counter_bytes += sizeof(double);
},
{
    number = *(double *)(code + counter_bytes);
    fprintf(disasm_list, " ");
    hex_print((unsigned char *) &number, sizeof(double), disasm_list);

    fprintf(disasm_list, "\"%*.c", FILE_LISTING_INDENT - 24, ' ');
    fprintf(disasm_list, "push %lg", number);
    fprintf(res, "push %lg", number);

    counter_bytes += sizeof(double);
})

DEF_CMD(POP, 110, 50737565,
{
    DO_POP;
},
{
    DISASM_LIST_PRINT(pop)
})

DEF_CMD(IN, 120, 46746,
{
    printf("Please, enter the number you want to push\n");
    scanf("%lf", &temp_1);

    DO_PUSH(temp_1);
},
{
    DISASM_LIST_PRINT(in)
})
