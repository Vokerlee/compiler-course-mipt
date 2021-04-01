#ifndef SORT_HEADER_H_INCLUDED
#define SORT_HEADER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <ctime>

#include "defines.h"

//-----------------------------------------------------------------------------
//! The structure of line
//! @version 10.0
//! @authors Romaaaaaa
//! @brief Includes the length of the line and the pointer to this line
//-----------------------------------------------------------------------------

struct line_t
{
    char *line;
    int length;
};

//-----------------------------------------------------------------------------
//! Exchange two structures
//! @param [in] l - the first structure
//! @param [in] r - the second structure
//! @version 10.0
//! @authors Romaaaaaa
//! @brief Exchanges two structures
//-----------------------------------------------------------------------------

void exch (line_t *lines, int l, int r);

//-----------------------------------------------------------------------------
//! Compare two lines (for ascending sort)
//! @param [in] line1 - the first line of some structure
//! @param [in] line2 - the second line of some structure
//! @param [out] a    - a > 0, if alphabetically line1 more than line2 and a < 0 respectively, a = 0 if (line1 = line2)
//! @version 20.0
//! @authors Romaaaaaa
//! @brief Compares two lines of structures (for ascending sort). Can be used in librarian qsort(...)
//-----------------------------------------------------------------------------

int strcomp_ascen (const void *line1, const void *line2);

//-----------------------------------------------------------------------------
//! Compare two lines (for descending sort)
//! @param [in] line1 - the first line of some structure
//! @param [in] line2 - the second line of some structure
//! @param [out] a    - a < 0, if alphabetically line1 more than line2 and a > 0 respectively, a = 0 if (line1 = line2)
//! @version 20.0
//! @authors Romaaaaaa
//! @brief compares two lines of structures (for descending sort). Can be used in librarian qsort(...)
//-----------------------------------------------------------------------------

int strcomp_descen (const void *line1, const void *line2);

//-----------------------------------------------------------------------------
//! Compare two lines (for rhyme sort)
//! @param [in] line1 - the first line of some structure
//! @param [in] line2 - the second line of some structure
//! @param [out] a    - a > 0, if alphabetically the last symbols in line1 more than line2 respectively, \
//! a < 0, if line2 > line1 and a = 0 if (line1 = line2)
//! @version 20.0
//! @authors Romaaaaaa
//! @brief compares two lines of structures (for rhyme sort). Can be used in librarian qsort(...)
//-----------------------------------------------------------------------------

int strcomp_rhyme (const void *line1, const void *line2);

//-----------------------------------------------------------------------------
//! Quick sort for structures
//! @param [in] lines    - the massive of structures
//! @param [in] l_0      - the first nubmer of sorting structures in massive
//! @param [in] r_0      - the second nubmer of sorting structures in massive
//! @param [in] str_comp - comparator of sort (acsending, descending or rhyme)
//! @version 11.0
//! @authors Romaaaaaa
//! @brief uses quick sort for structures, if the recursion r_0 - l_0
//-----------------------------------------------------------------------------

void quicksort (line_t *lines, int l_0, int r_0, int (*str_comp)(const void*, const void*));

//-----------------------------------------------------------------------------
//! Insertion sort for structures
//! @param [in] lines    - the massive of structures
//! @param [in] l_0      - the first nubmer of sorting structures in massive
//! @param [in] r_0      - the second nubmer of sorting structures in massive
//! @param [in] str_comp - comparator of sort (acsending, descending or rhyme)
//! @version 11.0
//! @authors Romaaaaaa
//! @brief uses insertion sort for structures, finish sort of structures after quicksort
//-----------------------------------------------------------------------------

void insertion (line_t *lines, int n, int (*str_comp)(const void*, const void*));

//-----------------------------------------------------------------------------
//! Sort the structures
//! @param [in] lines    - the massive of structures
//! @param [in] n        - the nubmer of sorting structures in massive
//! @param [in] str_comp - comparator of sort (acsending, descending or rhyme)
//! @version 12.0
//! @authors Romaaaaaa
//! @brief Combines two sorts - quick and then insertion
//-----------------------------------------------------------------------------

void megasort (line_t *lines, int n, int (*str_comp)(const void*, const void*));

//-----------------------------------------------------------------------------
//! Return the nubmer of symbols in source-file
//! @param [in] onegin - the the stream of source-file
//! @param [out] n     - the nubmer of symbols in source-file
//! @version 1.0
//! @authors Romaaaaaa
//! @brief Return the nubmer of symbols in source-file
//-----------------------------------------------------------------------------

int length_of_file(FILE *onegin);

//-----------------------------------------------------------------------------
//! Change of the value of lines_file to correct
//! @param [in] stream      - the massive of text in source-file
//! @param [in] lines_file  - the nubmer of lines in source-file
//! @param [in] length_file - the nubmer of symbols in source-file
//! @version 1.0
//! @authors Romaaaaaa
//! @brief Change the value of lines_file to correct
//-----------------------------------------------------------------------------

void lines_of_file(char *stream, size_t *lines_file, size_t *length_file);

//-----------------------------------------------------------------------------
//! Read file and write in massive
//! @param [in] onegin      - the stream of source-file
//! @param [in] file_length - the number of sybmols in source-file
//! @param [in] file_lines  - the number of lines in source-file
//! @param [out] stream     - the massive of text in source-file
//! @version 12.0
//! @authors Romaaaaaa
//! @brief Write text from source-file "onegin" in massive "stream" and return it\
//! changing the values "file_lines" and "file_length" to correct
//-----------------------------------------------------------------------------

char *read_file (FILE *onegin, int *file_length, int *file_lines);

//-----------------------------------------------------------------------------
//! Write all text-lines of source-file in structures
//! @param [in] lines      - the massive of structures
//! @param [in] stream     - the massive of text in source-file
//! @param [in] n_symb     - the counter of symbols in massive of structs "lines"
//! @param [in] n_struct   - the counter of lines in massive of structs "lines"
//! @param [in] file_length - the number of symbols in source-file
//! @version 12.0
//! @authors Romaaaaaa
//! @brief Write all non-empty lines from massive "stream" in massive of structs "lines"
//-----------------------------------------------------------------------------

void write_in_structs (line_t *lines, char *stream, size_t *n_symb, size_t *n_struct, size_t *file_length);

//-----------------------------------------------------------------------------
//! Choosing comparator of sort
//! @param [in] regime - the regime of sort (acsending, descending or rhyme)
//! @version 10.0
//! @authors Romaaaaaa
//! @brief Return the pointer to functions, based on regime of sort (comp_ascen, comp_descen, comp_rhyme)
//-----------------------------------------------------------------------------

int (*choose_comp(const char *regime))(const void *, const void *);

//-----------------------------------------------------------------------------
//! Sort the lines in stream onegin
//! @param [in] onegin   - the stream of source-file
//! @param [in] res      - the stream of results-file
//! @param [in] regime   - the regime of sort (acsending, descending or rhyme)
//! @param [in] bred_ind - the regime of bredogenerator ("bred+" to turn on)
//! @version 10.0
//! @authors Romaaaaaa
//! @brief Sort lines from "onegin" and print them in "res"
//-----------------------------------------------------------------------------

void sortlines (FILE *onegin, FILE *res, const char *regime, const char *bred_ind);

//-----------------------------------------------------------------------------
//! Sort the lines in stream onegin
//! @param [in] res      - the stream of results-file
//! @param [in] lines    - the massive of structures
//! @param [in] n_struct - the counter of lines in massive of structs "lines"
//! @version 10.0
//! @authors Romaaaaaa
//! @brief Print structures "lines" in "res"
//-----------------------------------------------------------------------------

void print_structs(FILE *res, line_t *lines, size_t n_struct);

//-----------------------------------------------------------------------------
//! Compare two text files
//! @param [in] res_prog   res_prog - the stream of the first file
//! @param [in] res_test   res_test - the stream of the second file
//! @version 1.4
//! @authors Romaaaaaa
//! @brief compares two files and returns 0, if they are different and 1 if not
//-----------------------------------------------------------------------------

int comptexts (FILE *res_prog, FILE *res_test);

//-----------------------------------------------------------------------------
//! Test to function sortlines()
//! @version 1.6 beta
//! @authors Romaaaaaa
//! @brief checks the function sortlines() for correctness                      \
//! The source of testing files should be saved in files like test1.txt (1 - 5) \
//! All correct results should be saved in files like res_test1.txt (1 - 5)     \
//! If you want to see the results of programs - open files res_prog1.txt (1 - 5)
//-----------------------------------------------------------------------------

void sortlines_test ();

//-----------------------------------------------------------------------------
//! Creat genious poem based on Romeo and Juliet
//! @param [in] lines - the massive of structures
//! @param [in] res   - the stream of results-file
//! @param [in] n     - the number of structures
//! @version 1.6 beta
//! @authors Romaaaaaa
//! @brief if the regime of sort if "rhyme" the program generates the most    \
//! beautiful poem on your life and print the results in bred.txt
//-----------------------------------------------------------------------------

void bred_generator (line_t *lines, const char* bred_ind, int n);


#endif // SORT_HEADER_H_INCLUDED
