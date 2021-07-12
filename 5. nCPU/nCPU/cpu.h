#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#define _USE_MATH_DEFINES
#define PI

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "stack.h"

#include "enum.h"

//-----------------------------------------------------------------------------
//! The struct of CPU
//! @version 1.0
//! @authors Romaaaaaa
//! @brief The struct of CPU, contains stack, bytes of code, code and registres
//-----------------------------------------------------------------------------

struct CPU {
    stack_t stk;
    stack_t stk_call;

    size_t bytes_cmd = 0;
    size_t n_labels  = 0;

    char *code  = nullptr;
    int *labels = nullptr;

    double rix[5] = {0};

    double RAM[100000];
};

//-----------------------------------------------------------------------------
//! Processes the byte-code and prints the results
//! @param [in] cpu    - the struct of CPU
//! @param [in] source - the stream of source-file (code)
//! @param [in] res    - the stream of results-file
//! @version 2.0
//! @authors Vokerlee
//! @brief Processes the byte-code and executes the necessary actions, prints the results in output.txt
//-----------------------------------------------------------------------------

void cpu_handler (CPU *cpu, FILE *source, FILE *res);

//-----------------------------------------------------------------------------
//! Processes the byte-code
//! @param [in] cpu - the struct of CPU (is already handled by function cpu_handler)
//! @param [in] res - the stream of results-file
//! @version 2.0
//! @authors Vokerlee
//! @brief Processes the byte-code and executes the necessary actions
//-----------------------------------------------------------------------------

void commands_handler (CPU *cpu, FILE *res);

//-----------------------------------------------------------------------------
//! Checks the code in source-file for valid ID
//! @param [in] source - the stream of source-file
//! @version 2.0
//! @authors Vokerlee
//! @brief Checks the code in source-file for valid ID
//-----------------------------------------------------------------------------

void id_verification(FILE *source);

#endif // CPU_H_INCLUDED
