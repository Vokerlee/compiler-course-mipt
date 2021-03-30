#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <text.h>

#include "tree_config.h"

#define ASSERT_TREE_OK             \
    if (error_tree(tree) != 0)    \
    {                              \
        dump_tree(tree);           \
        return nullptr;            \
    }

#define ASSERT_TREE_OK_VOID        \
    if (error_tree(tree) != 0)    \
    {                              \
        dump_tree(tree);           \
        return;                    \
    }

//-----------------------------------------------------------------------------
//! The structure of binary tree element
//! @version 1.0
//! @authors Vokerlee
//! @brief Consists of data - the name of element, left, right and previous elements
//-----------------------------------------------------------------------------

struct bin_tree_elem
{
    char *data = nullptr;

    bin_tree_elem *left = nullptr;

    bin_tree_elem *right = nullptr;

    bin_tree_elem *prev = nullptr;
};

//-----------------------------------------------------------------------------
//! The structure of binary tree
//! @version 1.0
//! @authors Vokerlee
//! @brief Consists of pointer to the root-element, size of tree, its name and error_state
//-----------------------------------------------------------------------------

struct bin_tree
{
    int tree_size = 0;

    int error_state = 0;

    bin_tree_elem *root = nullptr;

    char *tree_name = nullptr;
};

//-----------------------------------------------------------------------------
//! Tree-constructor
//! @param [in] tree - the structure of binary tree
//! @param [in] name - the name of binary_tree
//! @version 1.0
//! @authors Vokerlee
//! @brief Constructs binary tree, preparing it to be used
//-----------------------------------------------------------------------------

void construct_tree (bin_tree *tree, const char *name);

//-----------------------------------------------------------------------------
//! Tree-destructor
//! @param [in] tree - the structure of text
//! @version 1.0
//! @authors Vokerlee
//! @brief Destructs text, releasing all buff-memory
//-----------------------------------------------------------------------------

void destruct_tree (bin_tree *tree);

//-----------------------------------------------------------------------------
//! Deletes tree-elements
//! @param [in] element - the element of tree, memory of all its daughters which will be released
//! @version 1.0
//! @authors Vokerlee
//! @brief Deletes tree-elements, beginning from set element
//-----------------------------------------------------------------------------

void delete_tree_elem (bin_tree_elem *element);

//-----------------------------------------------------------------------------
//! Inserts the element in tree from left position of "element"
//! @param [in] tree    - the structure of tree
//! @param [in] element - the element of tree, from left position of which will be pushed new element with set data
//! @param [in] data    - the name (data) of pushed element
//! @version 1.0
//! @authors Vokerlee
//! @brief Inserts the element in tree from left position of "element" with set "data" value
//-----------------------------------------------------------------------------

bin_tree_elem *insert_left_tree (bin_tree *tree, bin_tree_elem *element, char *data);

//-----------------------------------------------------------------------------
//! Inserts the element in tree from right position of "element"
//! @param [in] tree    - the structure of tree
//! @param [in] element - the element of tree, from right position of which will be pushed new element with set data
//! @param [in] data    - the name (data) of pushed element
//! @version 1.0
//! @authors Vokerlee
//! @brief Inserts the element in tree from right position of "element" with set "data" value
//-----------------------------------------------------------------------------

bin_tree_elem *insert_right_tree (bin_tree *tree, bin_tree_elem *element, char *data);

//-----------------------------------------------------------------------------
//! Checks tree for valid size
//! @param [in] tree    - the structure of tree
//! @param [in] element - the element of tree (usually the root of tree)
//! @param [in] counter - the counter of tree-elements (is used to escape looping, counter = 0 - required initial value)
//! @version 1.0
//! @authors Vokerlee
//! @brief Checks tree for valid size
//-----------------------------------------------------------------------------

void check_size_tree (bin_tree *tree, bin_tree_elem *element, int *counter);

//-----------------------------------------------------------------------------
//! Checks tree for different errors
//! @param [in] tree - the structure of tree
//! @version 1.0
//! @authors Vokerlee
//! @brief Checks tree for different errors
//-----------------------------------------------------------------------------

int error_tree (bin_tree *tree);

//-----------------------------------------------------------------------------
//! Checks tree for valid links between its elements
//! @param [in] tree       - the structure of tree
//! @param [in] element    - the element of tree (usually the root of tree)
//! @param [in] counter    - the counter of tree-elements (is used to escape looping, counter = 0 - required initial value)
//! @param [in] link_state - the detector of tree-errors (is switched to 1, if there is link-error)
//! @version 1.0
//! @authors Vokerlee
//! @brief Checks tree for valid links between its elements
//-----------------------------------------------------------------------------

void check_links_tree (bin_tree *tree, bin_tree_elem *element, int *counter, int *link_state);

//-----------------------------------------------------------------------------
//! Prints all info about tree in case of its error (in tree_log.txt)
//! @param [in] tree - the structure of tree
//! @version 1.0
//! @authors Vokerlee
//! @brief Prints all info about tree in case of its error (in tree_log.txt)
//-----------------------------------------------------------------------------

void dump_tree (bin_tree *tree);

//-----------------------------------------------------------------------------
//! Prints all info for text-dump in "output"
//! @param [in] element - the element of tree (usually the root of tree)
//! @param [in] output  - the stream of output
//! @version 1.0
//! @authors Vokerlee
//! @brief Prints all info for text-dump in "output" (mainly is used for dump_tree)
//-----------------------------------------------------------------------------

void tree_print (bin_tree_elem *element, FILE *output);

//-----------------------------------------------------------------------------
//! Prints all info for graph-dump in "output"
//! @param [in] element - the element of tree (usually the root of tree)
//! @param [in] output  - the stream of output
//! @version 1.0
//! @authors Vokerlee
//! @brief Prints all info for graph-dump in "output" (graphviz-code, mainly is used for dump_tree)
//-----------------------------------------------------------------------------

void print_graph_vertex_tree (bin_tree_elem *element, FILE *output);

//-----------------------------------------------------------------------------
//! Returns the line of error-message
//! @param [in] error - the error, discovered in tree
//! @version 1.0
//! @authors Vokerlee
//! @brief Returns the line of error-message
//-----------------------------------------------------------------------------

const char *print_tree_error (int error);

#endif // TREE_H_INCLUDED
