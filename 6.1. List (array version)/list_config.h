#ifndef LIST_CONFIG_H_INCLUDED
#define LIST_CONFIG_H_INCLUDED

const int LIST_INIT_MAXCOUNT   = 5;
const int LIST_REALLOC_HYSTER  = 2;

const int LIST_LOGIC_STATE_ON  = 1;
const int LIST_LOGIC_STATE_OFF = 0;

const double POISON = NAN;

enum list_error
{
    LIST_INVALID_INIT_CAPACITY   = 1,
    LIST_LINK_ERROR              = 2,
    LIST_NEXT_ERR                = 3,
    LIST_PREV_ERR                = 4,
    LIST_CONST_RATIO_ERR         = 5,
    LIST_NULL_PTR                = 6,
    LIST_NULL_ELEM_PTR           = 7,
    LIST_INVALID_LIST_SIZE       = 8,
    LIST_INVALID_LIST_CAPACITY   = 9,
    LIST_INVALID_SIZE_CAP_RATIO  = 10,
    LIST_ERROR_DELETE            = 11,
    LIST_INVALID_INSERT_NUMBER   = 12,
    LIST_INVALID_INSERT_POSITION = 13,
    LIST_INVALID_DEL_POSITION    = 14,
    LIST_FREE_MEMORY_ERROR       = 15,
    LIST_INVALID_POS             = 16,
    LIST_INABLE_LOGIC_POS        = 17,
};

#endif // LIST_CONFIG_H_INCLUDED
