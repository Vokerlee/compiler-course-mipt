#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

const int STACK_INIT_MAXCOUNT   = 5;
const int STACK_MAX_NAME_LENGTH = 30;
const int STACK_REALLOC_HYSTER  = 2;
const int STACK_ADD_ALLOC       = 1;
const int STACK_RED_ALLOC       = 0;
const int ON                    = 1;
const int OFF                   = 0;

const int STACK_CANARY_LEFT       = 0xABCCBA1;
const int STACK_CANARY_RIGHT      = 0x14BBB88;
const int STACK_DATA_CANARY_LEFT  = 0xA2B2C8F;
const int STACK_DATA_CANARY_RIGHT = 0xFFF6623;

enum stack_errors
{
    STACK_NULL_PTR              = 1,
    STACK_DOUBLE_CONSTR         = 2,
    STACK_NULL_DATA_PTR         = 3,
    STACK_INVALID_CAPACITY      = 4,
    STACK_INVALID_COUNTER       = 5,
    STACK_CANARY_RIGHT_ERR      = 6,
    STACK_CANARY_LEFT_ERR       = 7,
    STACK_CANARY_DATA_LEFT_ERR  = 8,
    STACK_CANARY_DATA_RIGHT_ERR = 9,
    STACK_EMPTY_POP             = 10,
    STACK_HACKED                = 11,
    STACK_INVALID_PUSH          = 12,
};

#endif // CONFIG_H_INCLUDED
