#ifndef AKINATOR_H_INCLUDED
#define AKINATOR_H_INCLUDED

#include "akinator_config.h"

//-----------------------------------------------------------------------------
//! Launches akinator-game in console
//! @param [in] source - the source of akinator-base
//! @version 1.0
//! @authors Vokerlee
//! @brief Launches akinator-game in console
//-----------------------------------------------------------------------------

void akinator_menu (const char *source);

//-----------------------------------------------------------------------------
//! Fills tree from akinator-base
//! @param [in] tree - the structure of tree
//! @param [in] base - the stream of akinator-base
//! @version 1.0
//! @authors Vokerlee
//! @brief Fills tree from akinator-base
//-----------------------------------------------------------------------------

void fill_tree_from_base (bin_tree *tree, FILE *base);

//-----------------------------------------------------------------------------
//! Returns the length of file (the number of symbols)
//! @param [in] file - the stream of some file
//! @version 1.0
//! @authors Vokerlee
//! @brief Returns the length of file (the number of symbols)
//-----------------------------------------------------------------------------

int length_of_file (FILE *file);

//-----------------------------------------------------------------------------
//! Fills tree from analyses of text in buffer
//! @param [in]  tree    - the structure of tree
//! @param [in]  buffer  - the buffer, where the akinator-base is
//! @param [in]  counter - the pointer (is used to scan the buffer)
//! @param [out] root    - the root of tree
//! @version 1.0
//! @authors Vokerlee
//! @brief Fills tree from analyses of text in buffer
//-----------------------------------------------------------------------------

bin_tree_elem *create_tree (bin_tree *tree, char *buffer, int *counter);

//-----------------------------------------------------------------------------
//! Adds all links, connected with previous elements
//! @param [in] element - the element of tree (usually the root of tree)
//! @param [in] counter - the counter (is used to learn about tree-size)
//! @version 1.0
//! @authors Vokerlee
//! @brief Adds all links, connected with previous elements
//-----------------------------------------------------------------------------

void add_prev_links (bin_tree_elem *element, int *counter);

//-----------------------------------------------------------------------------
//! The regime of guessing the set object
//! @param [in] tree - the structure of tree
//! @version 1.0
//! @authors Vokerlee
//! @brief The regime of guessing the set object (is used in akinator_game function)
//-----------------------------------------------------------------------------

void object_guess (bin_tree *tree);

//-----------------------------------------------------------------------------
//! The regime of giving different questions about objects
//! @param [in] tree - the structure of tree
//! @param [in] element - the element of tree (usually the root of tree)
//! @version 1.0
//! @authors Vokerlee
//! @brief The regime of giving different questions about objects (is used in object_guess function)
//-----------------------------------------------------------------------------

void ask_questions (bin_tree *tree, bin_tree_elem *element);

//-----------------------------------------------------------------------------
//! The regime of comparing two different objects
//! @param [in] tree - the structure of tree
//! @version 1.0
//! @authors Vokerlee
//! @brief The regime of comparing two different objects (is used in akinator_game function)
//-----------------------------------------------------------------------------

void object_compare (bin_tree *tree);

//-----------------------------------------------------------------------------
//! The regime of giving all known facts about set object
//! @param [in] tree - the structure of tree
//! @version 1.0
//! @authors Vokerlee
//! @brief The regime of giving all known facts about set object (is used in akinator_game function)
//-----------------------------------------------------------------------------

void object_definition (bin_tree *tree);

//-----------------------------------------------------------------------------
//! Searches for set word
//! @param [in]  element       - the element of tree (usually the root of tree)
//! @param [in]  word          - the word, entered by user (some object)
//! @param [out] found_element - the found element, or nullptr, if it doesn't exist
//! @version 1.0
//! @authors Vokerlee
//! @brief Searches for set word (is used in different akinator function)
//-----------------------------------------------------------------------------

bin_tree_elem *search_word (bin_tree_elem *element, char *word);

//-----------------------------------------------------------------------------
//! Prints all properties of found element
//! @param [in]  element       - the element of tree (usually the root of tree)
//! @param [in]  word          - the word, entered by user (some object)
//! @param [out] found_element - the found element, or nullptr, if it doesn't exist
//! @version 1.0
//! @authors Vokerlee
//! @brief Prints all properties of found element (is used in object_definition function)
//-----------------------------------------------------------------------------

void print_element_properties (bin_tree_elem *element);

//-----------------------------------------------------------------------------
//! Makes and opens pdf file of akinator-base
//! @param [in] tree - the structure of tree
//! @version 1.0
//! @authors Vokerlee
//! @brief Makes and opens pdf file of akinator-base (is used in akinator_game function)
//-----------------------------------------------------------------------------

void show_base (bin_tree *tree);

//-----------------------------------------------------------------------------
//! Makes graphviz file to make pdf of akinator-base
//! @param [in] element - the element of tree (usually the root of tree)
//! @param [in] output  - the stream of graphviz file
//! @version 1.0
//! @authors Vokerlee
//! @brief Makes graphviz file to make pdf of akinator-base (is used in show_base function)
//-----------------------------------------------------------------------------

void print_base_tree (bin_tree_elem *element, FILE *output);

//-----------------------------------------------------------------------------
//! Fills akinator-base with changed (or not) akinator-tree (the shell of filling)
//! @param [in] tree - the structure of tree
//! @param [in] base - the stream of akinator-base file
//! @version 1.0
//! @authors Vokerlee
//! @brief Fills akinator-base with changed (or not) akinator-tree (is used in the end of akinator-game)
//-----------------------------------------------------------------------------

void fill_akinator_base (bin_tree *tree, FILE *base);

//-----------------------------------------------------------------------------
//! Fills akinator-base with changed (or not) akinator-tree (the foundation of filling)
//! @param [in] element - the element of tree (usually the root of tree)
//! @param [in] base    - the stream of akinator-base file
//! @param [in] indent  - the parameter of indentation for formatting the akinator-base file
//! @version 1.0
//! @authors Vokerlee
//! @brief Fills akinator-base with changed (or not) akinator-tree (is used in fill_akinator_base function)
//-----------------------------------------------------------------------------

void fill_base_elements (bin_tree_elem *element, FILE *base, int indent);

//-----------------------------------------------------------------------------
//! Prints set amount of indents
//! @param [in] base    - the stream of akinator-base file
//! @param [in] indent  - the amount of indents
//! @version 1.0
//! @authors Vokerlee
//! @brief Prints set amount of indents
//-----------------------------------------------------------------------------

inline void print_indent (FILE *base, int indent);


#endif // AKINATOR_H_INCLUDED
