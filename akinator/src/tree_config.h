#ifndef TREE_CONFIG_H_INCLUDED
#define TREE_CONFIG_H_INCLUDED

enum tree_error
{
    TREE_NULL_PTR               = 1,
    TREE_ROOT_NULL_PTR          = 2,
    TREE_INVALID_SIZE           = 3,
    TREE_REPEAT_INSERT_LEFT     = 4,
    TREE_REPEAT_INSERT_RIGHT    = 5,
    TREE_LINK_ERROR             = 6,
    TREE_INVALID_INSERT_ELEMENT = 7,
};

#endif // TREE_CONFIG_H_INCLUDED
