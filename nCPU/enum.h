#ifndef ENUM_H_INCLUDED
#define ENUM_H_INCLUDED

enum commands {
    #define DEF_CMD(name, num, hash, code_cpu, code_disasm) \
        name = num,

    #include "commands.h"

    #undef DEF_CMD
};

#endif // ENUM_H_INCLUDED
