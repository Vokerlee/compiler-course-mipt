# Linked list (array version)

This directory contains array version of linked list; array has the property of locality, which gives a wonderful possibility to use cash and to boost all functions.

All the functions (there descriptions) you can find in `list.h`.

There some unique things about this implementation of list:
* There is a function `void sort_list (linked_list *lst)`, which can sort the list (quick sort is used).
* There is a possibility to work logical positions, not physical. For is the function `void sort_next_list (linked_list *lst)` is used, which rearrange all elements in list, making the right order. (`sort_list` calls `sort_next_list`).
* All other functions have names, similar to STL.
