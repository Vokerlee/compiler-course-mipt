#ifndef STACK_HEAD_H_INCLUDED
#define STACK_HEAD_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <text.h>

#include "stack_config.h"

typedef double data_t;
#define SPECIFICATOR "lg"
const data_t POISON = NAN;

static int STACK_POP_STATE = 0;

#define CONSTRUCT_STACK(stk, n)                     \
    stack_t stk = {};                               \
    construct_stack(&stk, n, #stk);

#define STACK_ASSERT_OK                             \
    if (error_stack(stk))                           \
    {                                               \
        FILE *log = fopen("log_stack.txt", "ab");   \
        dump_stack(stk, log);                       \
        fclose(log);                                \
                                                    \
        return;                                     \
    }

#define STACK_ASSERT_POP_OK                         \
    if (error_stack(stk))                           \
    {                                               \
        FILE *log = fopen("log_stack.txt", "ab");   \
        dump_stack(stk, log);                       \
        fclose(log);                                \
                                                    \
        return POISON;                              \
    }

//-----------------------------------------------------------------------------
//! Struct of stack
//! @version 1.0
//! @authors Vokerlee
//! @brief Contains the data, capacity and current number of elements, hash and canaries
//-----------------------------------------------------------------------------

struct stack_t
{
    int canary_left = 0;

    int hash_number = 0;
    int error_state = 0;

    data_t *data = nullptr;
    int counter  = 0;
    int capacity = 0;

    char *stack_name = nullptr;

    int canary_right = 0;
};

//-----------------------------------------------------------------------------
//! Construct the stack
//! @param [in] stk      - the address of stack
//! @param [in] capacity - the number of elements in created stack (capacity >= 0)
//! @version 3.0
//! @authors Vokerlee
//! @brief Constructs stack for future using
//-----------------------------------------------------------------------------

void construct_stack (stack_t *stk, int capacity, const char *name);

//-----------------------------------------------------------------------------
//! Destructs the stack
//! @param [in] stk - the address of stack
//! @version 3.0
//! @authors Vokerlee
//! @brief Destructs stack, deleting all its information (is necessary at the end of using)
//-----------------------------------------------------------------------------

void destruct_stack (stack_t *stk);

//-----------------------------------------------------------------------------
//! Pushes the element in stack
//! @param [in] stk   - the address of stack
//! @param [in] value - pushing element in stack
//! @version 1.0
//! @authors Vokerlee
//! @brief Pushes the element value in stack
//-----------------------------------------------------------------------------

void push_stack (stack_t *stk, double value);

//-----------------------------------------------------------------------------
//! Pops the element in stack
//! @param [in]  stk   - the address of stack
//! @param [out] value - the poping element of stack
//! @version 1.0
//! @authors Vokerlee
//! @brief Deletes the last element of stack returning it
//-----------------------------------------------------------------------------

double pop_stack (stack_t *stk);

//-----------------------------------------------------------------------------
//! Returns the last element in stack
//! @param [in]  stk   - the address of stack
//! @param [out] value - the value if the last element in stack
//! @version 1.0
//! @authors Vokerlee
//! @brief Returns the last element in stack
//-----------------------------------------------------------------------------

double top_stack (stack_t *stk);

//-----------------------------------------------------------------------------
//! Checks stack for emptiness
//! @param [in]  stk   - the address of stack
//! @param [out] value - the poping element of stack
//! @version 1.0
//! @authors Vokerlee
//! @brief Returns 1, if the stack is empty, otherwise returns 0
//-----------------------------------------------------------------------------

int empty_stack (stack_t *stk);

//-----------------------------------------------------------------------------
//! Reallocates of stack-size
//! @param [in] stk    - stack
//! @param [in] regime - the regime of reallocation: ADD_ALLOC or RED_ALLOC
//! @version 1.0
//! @authors Vokerlee
//! @brief If in stack counter = maxcount, the size of stack increases in
//! REALLOC_HYSTER amount, if counter < maxcount / (2 * REALLOC_HYSTER), the size
//! of stack decreases in REALLOC_HYSTER amount
//-----------------------------------------------------------------------------

void realloc_stack (stack_t *stk, int regime);

//-----------------------------------------------------------------------------
//! Checks stack for errors
//! @param [in]  stk          - the address of stack
//! @param [out] error_number - the number of error of stack
//! @version 1.0
//! @authors Vokerlee
//! @brief Returns the number of error of stack
//-----------------------------------------------------------------------------

int error_stack (stack_t *stk);

//-----------------------------------------------------------------------------
//! Checks stack for errors
//! @param [in]  error      - the number of error
//! @param [out] error_text - the message of error
//! @version 1.0
//! @authors Vokerlee
//! @brief Returns the message of error for dump_stack
//-----------------------------------------------------------------------------

const char *error_text (int error);

//-----------------------------------------------------------------------------
//! Checks stack for double constructor
//! @param [in] stk - the address of stack
//! @version 1.0
//! @authors Vokerlee
//! @brief Returns 0 if there is double constructor and 1 otherwise
//-----------------------------------------------------------------------------

int double_constr_error_stack (stack_t *stk);

//-----------------------------------------------------------------------------
//! Prints the information about stack
//! @param [in] stk - the address of stack
//! @param [in] log - the path, where the information for stack will be published
//! @version 1.0
//! @authors Vokerlee
//! @brief Prints all information about stack in log or in case of error in log
//-----------------------------------------------------------------------------

void dump_stack (stack_t *stk, FILE *log);

//-----------------------------------------------------------------------------
//! Recalculates the hash number
//! @param [in] stk - stack
//! @version 1.0
//! @authors Vokerlee
//! @brief Recalculates the hash number of stack for safety of stack-data
//-----------------------------------------------------------------------------

int recalculate_hash_stack (stack_t *stk);

//-----------------------------------------------------------------------------
//! Calculates the hash number
//! @param [in] stk   - stack
//! @param [out] hash - the valid hash-number of stack
//! @version 1.0
//! @authors Vokerlee
//! @brief Calculates the hash number in standart functions of stack for safety
//-----------------------------------------------------------------------------

void update_hash_stack (stack_t *stk);

#endif // STACK_HEAD_H_INCLUDED
