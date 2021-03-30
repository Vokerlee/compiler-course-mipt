#ifndef DIFF_H_INCLUDED
#define DIFF_H_INCLUDED

#include <text.h>
#include <ctype.h>
#include <time.h>

#include "tree.h"

#include "diff_config.h"

struct variables
{
    int n_var = 0;

    char var[MAX_VAR_NUM][MAX_VAR_NAME_LENGTH] = {0};
};

struct elements
{
    bin_tree_elem **elements = nullptr;

    int curr_size = 0;
};

struct subst_elem
{
    bin_tree_elem *subst = nullptr;

    int repeat_num = 0;
};

struct substitutions
{
    int curr_size = 0;
    int capacity  = 0;

    subst_elem *subst = nullptr;
};

//-----------------------------------------------------------------------------
//! Creates article of derivation
//! @param [in] formula - the stream, where the initial formula is
//! @version 1.0
//! @authors Vokerlee
//! @brief Creates article of derivation and opens it
//-----------------------------------------------------------------------------

void create_diff_article (FILE *formula);

//-----------------------------------------------------------------------------
//! Creates tree by analyses of text in formula
//! @param [in] tree    - the structure of tree
//! @param [in] formula - the stream, where the initial formula is
//! @param [in] var     - the massive of variables
//! @param [in] elem    - the massive of all tree-elements (to release all memory in critical situation)
//! @version 1.0
//! @authors Vokerlee
//! @brief Creates tree by analyses of text in formula
//-----------------------------------------------------------------------------

void fill_tree (bin_tree *tree, FILE *formula, variables *var, elements *elem);

//-----------------------------------------------------------------------------
//! Analyses maths' expression wholly and returns the root of tree-expression
//! @param [in]  var  - the massive of variables
//! @param [in]  elem - the massive of all tree-elements (to release all memory in critical situation)
//! @param [in]  text - the structure of text (where initial formula is)
//! @param [out] root - the pointer of tree-root
//! @version 1.0
//! @authors Vokerlee
//! @brief Analyses maths' expression and returns the root of tree-expression
//-----------------------------------------------------------------------------

bin_tree_elem *create_e_tree (variables *var, elements *elem, text_t *text);

//-----------------------------------------------------------------------------
//! Analyses maths' blocks, divided by symbols '*' and '/' and returns the pointer to this expression
//! @param [in]  var  - the massive of variables
//! @param [in]  elem - the massive of all tree-elements (to release all memory in critical situation)
//! @param [in]  text - the structure of text (where initial formula is)
//! @param [out] root - the pointer to analyzed blocks
//! @version 1.0
//! @authors Vokerlee
//! @brief Analyses maths' blocks, divided by symbols '*' and '/' and returns the pointer to this expression
//-----------------------------------------------------------------------------

bin_tree_elem *create_t_tree (variables *var, elements *elem, text_t *text);

//-----------------------------------------------------------------------------
//! Analyses maths' blocks, divided by symbol '^' and returns the pointer to this expression
//! @param [in]  var  - the massive of variables
//! @param [in]  elem - the massive of all tree-elements (to release all memory in critical situation)
//! @param [in]  text - the structure of text (where initial formula is)
//! @param [out] root - the pointer to analyzed blocks
//! @version 1.0
//! @authors Vokerlee
//! @brief Analyses maths' blocks, divided by symbol '^' and returns the pointer to this expression
//-----------------------------------------------------------------------------

bin_tree_elem *create_w_tree (variables *var, elements *elem, text_t *text);

//-----------------------------------------------------------------------------
//! Analyses maths' blocks of the form '(' f(x1,...x2) ')' and numbers, variables and elementary functions
//! @param [in]  var  - the massive of variables
//! @param [in]  elem - the massive of all tree-elements (to release all memory in critical situation)
//! @param [in]  text - the structure of text (where initial formula is)
//! @param [out] root - the pointer to analyzed blocks
//! @version 1.0
//! @authors Vokerlee
//! @brief Analyses maths' blocks, of the form '(' f(x1,...x2) ')' and numbers, variables and elementary functions
//-----------------------------------------------------------------------------

bin_tree_elem *create_p_tree (variables *var, elements *elem, text_t *text);

//-----------------------------------------------------------------------------
//! Analyses numbers, variables and elementary functions
//! @param [in]  var  - the massive of variables
//! @param [in]  elem - the massive of all tree-elements (to release all memory in critical situation)
//! @param [in]  text - the structure of text (where initial formula is)
//! @param [out] root - the pointer to analyzed expression
//! @version 1.0
//! @authors Vokerlee
//! @brief Analyses numbers, variables and elementary functions
//-----------------------------------------------------------------------------

bin_tree_elem *create_n_tree (variables *var, elements *elem, text_t *text);

//-----------------------------------------------------------------------------
//! Searches the element in variables and returns the number of it, if the element is in known variables
//! @param [in]  var        - the massive of variables
//! @param [in]  text       - the structure of text (where initial formula is)
//! @param [in]  elem       - the massive of all tree-elements (to release all memory in critical situation)
//! @param [out] var_number - the number of variable in massive, or -1, if there is no any var such as analyzed
//! @version 1.0
//! @authors Vokerlee
//! @brief Searches the element in variables and returns the number of it, if the element is in known variables, otherwise remembers it
//-----------------------------------------------------------------------------

int var_search (variables *var, text_t *text, elements *elem);

//-----------------------------------------------------------------------------
//! Creates the tree of derivative of initial tree and prints it in pdf (LeTeX)
//! @param [in] diff_tree - the tree of derivative
//! @param [in] text      - the initial tree of expression
//! @param [in] var       - the massive of variables
//! @version 1.0
//! @authors Vokerlee
//! @brief Creates the tree of derivate of initial tree and prints it in pdf (LeTeX)
//-----------------------------------------------------------------------------

void print_diff_tex (bin_tree *diff_tree, bin_tree *tree, variables *var);

//-----------------------------------------------------------------------------
//! Prints formula in tex
//! @param [in] element  - the element of tree (usually the root of tree)
//! @param [in] tex      - the stream of LaTeX document
//! @param [in] var      - the massive of variables
//! @param [in] priority - the priority of expression (is used for printing brackets)
//! @version 1.0
//! @authors Vokerlee
//! @brief Prints formula in tex, using the pointer of tree-root-expression
//-----------------------------------------------------------------------------

void print_formula (bin_tree_elem *element, FILE *tex, variables *var, int priority);

//-----------------------------------------------------------------------------
//! Compares two priorities of two set elements
//! @param [in]  element1 - the first  element
//! @param [in]  element2 - the second element
//! @param [out] priority - the priority of two elements
//! @version 1.0
//! @authors Vokerlee
//! @brief Compares two priorities of two set elements
//-----------------------------------------------------------------------------

int compare_priorities (bin_tree_elem *element1, bin_tree_elem *element2);

//-----------------------------------------------------------------------------
//! Creates the tree-element with set parameters
//! @param [in]  type  - the first  element
//! @param [in]  value - the second element
//! @param [in]  left  - the pointer to left daughter
//! @param [in]  right - the pointer to right daughter
//! @param [out] root  - the pointer to created element
//! @version 1.0
//! @authors Vokerlee
//! @brief Creates the tree-element with set parameters and returns the pointer to this element
//-----------------------------------------------------------------------------

bin_tree_elem *create_tree_element (int type, double value, bin_tree_elem *left, bin_tree_elem *right);

//-----------------------------------------------------------------------------
//! Creates copied element
//! @param [in]  element2      - the initial element
//! @param [out] coped_element - the pointer to created element
//! @version 1.0
//! @authors Vokerlee
//! @brief Creates copied element and returns the pointer to it
//-----------------------------------------------------------------------------

bin_tree_elem *copy_tree_elem (bin_tree_elem *elem2);

//-----------------------------------------------------------------------------
//! Creates copied tree
//! @param [in]  element       - the root of copying tree
//! @param [out] coped_element - the pointer to copied tree
//! @version 1.0
//! @authors Vokerlee
//! @brief Creates copied tree and returns the pointer to it
//-----------------------------------------------------------------------------

bin_tree_elem *copy_tree (bin_tree_elem *element);

//-----------------------------------------------------------------------------
//! Checks the tree for the fact that tree contains variables
//! @param [in] element - the root of checking tree
//! @version 1.0
//! @authors Vokerlee
//! @brief Checks the tree for the fact that tree contains variables
//-----------------------------------------------------------------------------

int is_elem_func (bin_tree_elem *element);

//-----------------------------------------------------------------------------
//! Prints the article about derivative in LaTeX language, creating the derivative-tree
//! @param [in]  element         - the root of checking tree
//! @param [in]  tex             - the stream of LaTeX document
//! @param [in]  var             - the massive of variables
//! @param [out] derivative_root - the root of derivative-tree
//! @version 1.0
//! @authors Vokerlee
//! @brief Prints the article about derivative in LaTeX language, creating the derivative-tree
//-----------------------------------------------------------------------------

bin_tree_elem *diff_tree_elem (bin_tree_elem *element, FILE *tex, variables *var);

//-----------------------------------------------------------------------------
//! Calculates the hash numbers of all elements of derivative-tree
//! @param [in]  element - the root of hashing tree
//! @param [out] root_hash - the hash of the root of hashing tree (side effect of recursion)
//! @version 1.0
//! @authors Vokerlee
//! @brief Calculates the hash numbers of all elements of derivative-tree (for following substitutions)
//-----------------------------------------------------------------------------

int hash_elem_count (bin_tree_elem *element);

//-----------------------------------------------------------------------------
//! Calculates the size of all elements of derivative-tree
//! @param [in]  element   - the root of the tree
//! @param [out] root_size - the size of the root of tree (side effect of recursion)
//! @version 1.0
//! @authors Vokerlee
//! @brief Calculates the size of all elements of derivative-tree (for following substitutions)
//-----------------------------------------------------------------------------

int size_elem_count (bin_tree_elem *element);

//-----------------------------------------------------------------------------
//! Creates substitutions in derivative-tree
//! @param [in] tree - the tree of derivative
//! @param [in] sub  - the massive of substitutions (is empty at the beginning)
//! @version 1.0
//! @authors Vokerlee
//! @brief Creates substitutions in derivative-tree (for following substitutions)
//-----------------------------------------------------------------------------

void create_substitutions (bin_tree *tree, substitutions *sub);

//-----------------------------------------------------------------------------
//! Makes preliminary substitutions
//! @param [in] element - the root of the tree
//! @param [in] prev    - the previous element of "element" (initial is nullptr, is used for recursion)
//! @param [in] sub     - the massive of substitutions (including necessary and unnecessary replacements)
//! @param [in] link    - the kind of linking between current and previous elements (LEFT, RIGHT, VERTEX)
//! @version 1.0
//! @authors Vokerlee
//! @brief Makes preliminary substitutions (!!!is used only by create_substitutions-function!!!)
//-----------------------------------------------------------------------------

void make_substitutions (bin_tree_elem *element, bin_tree_elem *prev, substitutions *sub, int link);

//-----------------------------------------------------------------------------
//! Recalculates the number of substitutions, already without unnecessary replacements
//! @param [in] element - the root of the tree
//! @param [in] prev    - the previous element of "element" (initial is nullptr, is used for recursion)
//! @param [in] sub     - the massive of substitutions (including necessary and unnecessary replacements)
//! @version 1.0
//! @authors Vokerlee
//! @brief Recalculates the number of substitutions, already without unnecessary replacements (!!!is used only by create_substitutions-function!!!)
//-----------------------------------------------------------------------------

void recalculate_repeat_num_subst (bin_tree_elem *element, bin_tree_elem *prev, substitutions *sub);

//-----------------------------------------------------------------------------
//! Deletes all unnecessary replacements
//! @param [in] element - the root of the tree
//! @param [in] prev    - the previous element of "element" (initial is nullptr, is used for recursion)
//! @param [in] sub     - the massive of substitutions
//! @param [in] link    - the kind of linking between current and previous elements (LEFT, RIGHT, VERTEX)
//! @version 1.0
//! @authors Vokerlee
//! @brief Deletes all unnecessary replacements (!!!is used only by create_substitutions-function!!!)
//-----------------------------------------------------------------------------

void delete_unuseful_subst (bin_tree_elem *element, bin_tree_elem *prev, substitutions *sub, int link);

//-----------------------------------------------------------------------------
//! Deletes all replacements of the tree
//! @param [in] element - the root of the tree
//! @param [in] prev    - the previous element of "element"
//! @param [in] link    - the kind of linking between current and previous elements (LEFT, RIGHT, VERTEX)
//! @version 1.0
//! @authors Vokerlee
//! @brief Deletes all replacements of the tree
//-----------------------------------------------------------------------------

void delete_all_subst (bin_tree_elem *element, bin_tree_elem *prev, int link);

//-----------------------------------------------------------------------------
//! Checks the element for the fact that it is already known substitution
//! @param [in]  sub     - the massive of substitutions
//! @param [in]  element - the root of the tree
//! @param [out] index   - the index of substitution in the massive if substitution (or -1)
//! @version 1.0
//! @authors Vokerlee
//! @brief Checks the element for the fact that it is already known substitution
//-----------------------------------------------------------------------------

int search_in_substitutions (substitutions *sub, bin_tree_elem *element);

//-----------------------------------------------------------------------------
//! Searches all potential substitutions (unnecessary and necessary)
//! @param [in] element - the root of the tree
//! @param [in] sub     - the massive of substitutions
//! @version 1.0
//! @authors Vokerlee
//! @brief Searches all potential substitutions (unnecessary and necessary)
//-----------------------------------------------------------------------------

void substitutions_search (bin_tree_elem *element, substitutions *sub);

//-----------------------------------------------------------------------------
//! Prints the name of function
//! @param [in] value - the code of function (diff_config.h)
//! @param [in] tex   - the stream of TeX-file
//! @version 1.0
//! @authors Vokerlee
//! @brief Prints the name of function
//-----------------------------------------------------------------------------

void print_func_text (int value, FILE *tex);

//-----------------------------------------------------------------------------
//! Generates random phrase for condition sentence
//! @param [out] random_phrase
//! @version 1.0
//! @authors Vokerlee
//! @brief Generates random phrase for condition sentence
//-----------------------------------------------------------------------------

const char *phrase_cond_print (void);

//-----------------------------------------------------------------------------
//! Generates random phrase for equality sentence
//! @param [out] random_phrase
//! @version 1.0
//! @authors Vokerlee
//! @brief Generates random phrase for equality sentence
//-----------------------------------------------------------------------------

const char *phrase_eq_print (void);

//-----------------------------------------------------------------------------
//! Handles the error in initial formula
//! @param [in] text - the structure of text
//! @param [in] elem - the massive of all tree-elements (to release all memory in critical situation)
//! @param [in] line - the number of line, where the message was detected
//! @param [in] file - the name of file, where the error was detected
//! @version 1.0
//! @authors Vokerlee
//! @brief Handles the error in initial formula
//-----------------------------------------------------------------------------

void syntax_error (text_t *text, elements *elem, int line, const char *file);

#endif // DIFF_H_INCLUDED
