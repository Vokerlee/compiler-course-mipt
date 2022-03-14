#ifndef P_LIST_CONFIG_H_INCLUDED
#define P_LIST_CONFIG_H_INCLUDED

const double POISON = NAN;

enum p_list_error
{
    P_LIST_INVALID_INIT_CAPACITY   = 1,
    P_LIST_LINK_ERROR              = 2,
    P_LIST_NEXT_ERR                = 3,
    P_LIST_PREV_ERR                = 4,
    P_LIST_CONST_RATIO_ERR         = 5,
    P_LIST_NULL_PTR                = 6,
    P_LIST_NULL_ELEM_PTR           = 7,
    P_LIST_INVALID_LIST_SIZE       = 8,
    P_LIST_INVALID_LIST_CAPACITY   = 9,
    P_LIST_INVALID_SIZE_CAP_RATIO  = 10,
    P_LIST_ERROR_DELETE            = 11,
    P_LIST_INVALID_INSERT_NUMBER   = 12,
    P_LIST_INVALID_INSERT_POSITION = 13,
    P_LIST_INVALID_DEL_POSITION    = 14,
    P_LIST_FREE_MEMORY_ERROR       = 15,
};

#endif // P_LIST_CONFIG_H_INCLUDED
