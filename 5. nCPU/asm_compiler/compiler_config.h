#ifndef COMPILER_CONFIG_H_INCLUDED
#define COMPILER_CONFIG_H_INCLUDED

const int FILE_LISTING_INDENT = 40;

const int ERROR_FORMAT      = 1;
const int ERROR_UNKNOWN_CMD = 2;

const int ERROR_HASH   = -1;
const int ERROR_READ   = -2;

enum command_type
{
    READ_LABEL        = 1,
    READ_CMD_LABEL    = 2,
    RAM_CMD           = 3,
    RAM_CMD_NUM       = 4,
    NO_ARGUMENT_CMD   = 5,
    ARGUMENT_CMD      = 6,
};

const int DATA_SIZE = 43;

char asm_data[DATA_SIZE] = {
    #define DEF_CMD(name, num, hash, code_cpu, code_disasm) \
        num,

    #include "commands.h"

    #undef DEF_CMD
};

int hash_data[DATA_SIZE] = {
    #define DEF_CMD(name, num, hash, code_cpu, code_disasm) \
        hash,

    #include "commands.h"

    #undef DEF_CMD
};

#endif // COMPILER_CONFIG_H_INCLUDED
