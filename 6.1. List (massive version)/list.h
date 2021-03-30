#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include <text.h>

#include "list_config.h"

#define LIST_CONSTR(list, size)             \
    linked_list list = {};                  \
    construct_list(&list, size, #list);

//-----------------------------------------------------------------------------
//! Struct of list-element
//! @version 1.0
//! @authors Vokerlee
//! @brief Contains the data of struct-element, address of next and previous element
//-----------------------------------------------------------------------------

struct linked_list_el
{
    double data = 0;

    int next = 0;

    int prev = 0;
};

//-----------------------------------------------------------------------------
//! Struct of list
//! @version 1.0
//! @authors Vokerlee
//! @brief Contains massive of linked_list_el's, capacity and current size of massive;
//! Contains addresses of next and previous elements; the next free element in massive and the name of list; logic_state - if = 1
//! The user can work with logical positions
//-----------------------------------------------------------------------------

struct linked_list
{
    int curr_size = 0;
    int capacity  = 0;

    int list_front = 0;
    int list_back  = 0;

    int next_free   = 0;
    int logic_state = 0;

    int error_state = 0;

    linked_list_el *elem = nullptr;
    char *list_name      = nullptr;
};

//-----------------------------------------------------------------------------
//! Constructs the struct of list
//! @param [in] lst      - the struct of list
//! @param [in] capacity - the initial capacity of list
//! @param [in] name     - the name of list (is used in dump)
//! @version 1.0
//! @authors Vokerlee
//! @brief Constructs the struct of list (prepares it for usage)
//-----------------------------------------------------------------------------

void construct_list (linked_list *lst, int capacity, const char *name);

//-----------------------------------------------------------------------------
//! Destructs the struct of list
//! @param [in] lst - the struct of list
//! @version 1.0
//! @authors Vokerlee
//! @brief Destructs the struct of list (releases all unuseful memory and poisons all variables)
//-----------------------------------------------------------------------------

void destruct_list (linked_list *lst);

//-----------------------------------------------------------------------------
//! Reallocates memory
//! @param [in] lst - the struct of list
//! @version 1.0
//! @authors Vokerlee
//! @brief Reallocates memory, if it is already small to store all data
//-----------------------------------------------------------------------------

void realloc_list (linked_list *lst);

//-----------------------------------------------------------------------------
//! Inserts element value before the element with position pos
//! @param [in]  lst         - the struct of list
//! @param [in]  pos         - the position of the element, before which we want to insert element
//! @param [in]  value       - the value we push in list
//! @param [in]  logic_state - the possibility to work with logical positions
//! @param [out] insert_pos  - the physical position where we insert the element
//! @version 1.0
//! @authors Vokerlee
//! @brief Inserts element value before element with position pos
//-----------------------------------------------------------------------------

int insert_before_list (linked_list *lst, int pos, double value, int logic_state = LIST_LOGIC_STATE_OFF);

//-----------------------------------------------------------------------------
//! Inserts element value after the element with position pos
//! @param [in]  lst         - the struct of list
//! @param [in]  pos         - the position of the element, after which we want to insert element
//! @param [in]  value       - the value we push in list
//! @param [in]  logic_state - the possibility to work with logical positions
//! @param [out] insert_pos  - the physical position where we insert the element
//! @version 1.0
//! @authors Vokerlee
//! @brief Inserts element value after element with position pos
//-----------------------------------------------------------------------------

int insert_after_list (linked_list *lst, int pos, double value, int logic_state = LIST_LOGIC_STATE_OFF);

//-----------------------------------------------------------------------------
//! Inserts element value in the end of the list
//! @param [in] lst   - the struct of list
//! @param [in] value - the value we push in list
//! @version 1.0
//! @authors Vokerlee
//! @brief Inserts element value in the end of the list
//-----------------------------------------------------------------------------

int push_front_list (linked_list *lst, double value);

//-----------------------------------------------------------------------------
//! Inserts element value in the beginning of the list
//! @param [in] lst   - the struct of list
//! @param [in] value - the value we push in list
//! @version 1.0
//! @authors Vokerlee
//! @brief Inserts element value in the beginning of the list
//-----------------------------------------------------------------------------

int push_back_list (linked_list *lst, double value);

//-----------------------------------------------------------------------------
//! Deletes the element in the list
//! @param [in] lst         - the struct of list
//! @param [in] pos         - the position of the element, we want to delete
//! @param [in] logic_state - the possibility to work with logical positions
//! @version 1.0
//! @authors Vokerlee
//! @brief Deletes the element in list
//-----------------------------------------------------------------------------

void erase_list (linked_list *lst, int pos, int logic_state = LIST_LOGIC_STATE_OFF);

//-----------------------------------------------------------------------------
//! Deletes the element in the beginning of the list
//! @param [in] lst         - the struct of list
//! @param [in] logic_state - the possibility to work with logical positions
//! @version 1.0
//! @authors Vokerlee
//! @brief Deletes the element in the beginning of the list
//-----------------------------------------------------------------------------

void pop_front_list (linked_list *lst, int logic_state = LIST_LOGIC_STATE_OFF);

//-----------------------------------------------------------------------------
//! Deletes the element in the end of the list
//! @param [in] lst         - the struct of list
//! @param [in] logic_state - the possibility to work with logical positions
//! @version 1.0
//! @authors Vokerlee
//! @brief Deletes the element in the end of the list
//-----------------------------------------------------------------------------

void pop_back_list (linked_list *lst, int logic_state = LIST_LOGIC_STATE_OFF);

//-----------------------------------------------------------------------------
//! Deletes all elements in the list
//! @param [in]  lst - the struct of list
//! @version 1.0
//! @authors Vokerlee
//! @brief Deletes all elements in the list
//-----------------------------------------------------------------------------

void clear_list (linked_list *lst);

//-----------------------------------------------------------------------------
//! Returns the position of the first element in list
//! @param [in]  lst - the struct of list
//! @param [out] pos - the position of the first element
//! @version 1.0
//! @authors Vokerlee
//! @brief Returns the position of the first element in list
//-----------------------------------------------------------------------------

int front_list (linked_list *lst);

//-----------------------------------------------------------------------------
//! Returns the position of the last element in list
//! @param [in]  lst - the struct of list
//! @param [out] pos - the position of the last element
//! @version 1.0
//! @authors Vokerlee
//! @brief Returns the position of the last element in list
//-----------------------------------------------------------------------------

int back_list (linked_list *lst);

//-----------------------------------------------------------------------------
//! Returns the position of the element after pos
//! @param [in]  lst         - the struct of list
//! @param [in]  pos         - the position of the element after pos
//! @param [in]  logic_state - the possibility to work with logical positions
//! @param [out] pos_after   - the position of the element, which is situated after element with position pos
//! @version 1.0
//! @authors Vokerlee
//! @brief Returns the position of the element, which is situated after element with position pos
//-----------------------------------------------------------------------------

int pos_after_list (linked_list *lst, int pos, int logic_state = LIST_LOGIC_STATE_OFF);

//-----------------------------------------------------------------------------
//! Returns the position of the element before pos
//! @param [in]  lst         - the struct of list
//! @param [in]  pos         - the position of the element after pos
//! @param [in]  logic_state - the possibility to work with logical positions
//! @param [out] pos_before  - the position of the element, which is situated before element with position pos
//! @version 1.0
//! @authors Vokerlee
//! @brief Returns the position of the element, which is situated before element with position pos
//-----------------------------------------------------------------------------

int pos_before_list (linked_list *lst, int pos, int logic_state = LIST_LOGIC_STATE_OFF);

//-----------------------------------------------------------------------------
//! Search for the position of element with data value
//! @param [in]  lst   - the struct of list
//! @param [in]  value - the value
//! @param [out] pos   - the position of the element, which data is equal to value
//! @version 1.0
//! @authors Vokerlee
//! @brief Returns the position of the element, which data is equal to value
//-----------------------------------------------------------------------------

int find_logic_pos_list (linked_list *lst, double value);

//-----------------------------------------------------------------------------
//! Returns the value of element with position = pos
//! @param [in]  lst       - the struct of list
//! @param [in]  logic_pos - the logical position in list
//! @param [out] pos       - the position of the element, which data is equal to value
//! @version 1.0
//! @authors Vokerlee
//! @brief Returns the position of the element, which logical position is logic_pos
//-----------------------------------------------------------------------------

int find_pos_list (linked_list *lst, int logic_pos);

//-----------------------------------------------------------------------------
//! Searches for errors in list
//! @param [in] lst    - the struct of list
//! @param [out] error - the number of error
//! @version 1.0
//! @authors Vokerlee
//! @brief Returns the code of error (look list_config.h) or 0 if everything is OK
//-----------------------------------------------------------------------------

int error_list (linked_list *lst);

//-----------------------------------------------------------------------------
//! Prints all information about list
//! @param [in] lst - the struct of list
//! @version 1.0
//! @authors Vokerlee
//! @brief Prints all information about list in log_list.txt and creates graph.pdf of list, opens them
//-----------------------------------------------------------------------------

void dump_list (linked_list *lst);

//-----------------------------------------------------------------------------
//! Exchanges physical positions of elements in list
//! @param [in] lst  - the struct of list
//! @param [in] pos1 - the position of the first element
//! @param [in] pos2 - the position of the second element
//! @version 1.0
//! @authors Vokerlee
//! @brief Exchanges physical positions of elements in list
//-----------------------------------------------------------------------------

void exch_phys_list (linked_list *lst, int pos1, int pos2);

//-----------------------------------------------------------------------------
//! Exchanges elements with logical positions pos1 and pos2
//! @param [in] lst  - the struct of list
//! @param [in] pos1 - the position of the first element
//! @param [in] pos2 - the position of the second element
//! @version 1.0
//! @authors Vokerlee
//! @brief Exchanges physical positions of elements in list
//-----------------------------------------------------------------------------

void exch_logic_list (linked_list *lst, int pos1, int pos2);

//-----------------------------------------------------------------------------
//! Compares elements
//! @param [in] lst   - the struct of list
//! @param [in] elem1 - the position of the first element
//! @param [in] elem2 - the position of the second element
//! @version 1.0
//! @authors Vokerlee
//! @brief Compares elements and return elem1.data - elem2.data
//-----------------------------------------------------------------------------

int elem_comp_list (const void *elem1, const void *elem2);

//-----------------------------------------------------------------------------
//! Sorts the elements in list
//! @param [in] lst   - the struct of list
//! @version 1.0
//! @authors Vokerlee
//! @brief Sorts the elements in list
//-----------------------------------------------------------------------------

void sort_list (linked_list *lst);

//-----------------------------------------------------------------------------
//! Is used by function sort to sort the elements with quick sort
//! @param [in] lst            - the struct of list
//! @param [in] l_0            - the first element of list
//! @param [in] r_0            - the last element of list
//! @param [in] list_elem_comp - the comparator of element in list
//! @warning Before usage of this function the list must be sort in "next" elements with function list_sort_next
//! @version 1.0
//! @authors Vokerlee
//! @brief Is used by function sort to sort the elements with quick sort
//-----------------------------------------------------------------------------

void qsort_list (linked_list *lst, int l_0, int r_0, int (*list_elem_comp)(const void*, const void*));

//-----------------------------------------------------------------------------
//! Opens the possibility to work with logical positions
//! @param [in] lst            - the struct of list
//! @version 1.0
//! @authors Vokerlee
//! @brief Sorts the elements of list and opens the possibility to work with logical positions
//-----------------------------------------------------------------------------

void sort_next_list (linked_list *lst);

#endif // LIST_H_INCLUDED
