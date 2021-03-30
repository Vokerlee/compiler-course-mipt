#ifndef COMPILE_H_INCLUDED
#define COMPILE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <text.h>

#include "enum.h"

//-----------------------------------------------------------------------------
//! The struct, containing all asm-labels
//! @version 1.0
//! @authors Vokerlee
//! @brief The struct of all labels in code.
//! Contains the number of labels n_labels, all labels' names and all labels' addresses
//-----------------------------------------------------------------------------

struct label
{
    char **name_lab = nullptr;

    int *numbers = nullptr;

    int n_labels = 0;

    int counter = 0;
};

//-----------------------------------------------------------------------------
//! Processes the code in compiled code
//! @param [in] text - the struct of text of source-code
//! @param [in] res  - the stream of result-file
//! @version 2.0
//! @authors Vokerlee
//! @brief Processes the code from struct text in compiled_code (in result-file)
//-----------------------------------------------------------------------------

void compile (text_t *text, FILE *res);

//-----------------------------------------------------------------------------
//! Prints header of file_listing
//! @param [in] file_listing - the stream of file_listing-file
//! @version 2.0
//! @authors Vokerlee
//! @brief Prints header of file_listing
//-----------------------------------------------------------------------------

void print_file_listing_header (FILE *file_listing);

//-----------------------------------------------------------------------------
//! Fills label-struct with all label-addresses in code
//! @param [in] labels - the massive of binary code, where we put all labels' addresses
//! @param [in] line   - the line of command
//! @version 2.0
//! @authors Vokerlee
//! @brief Fills label-struct with all label-addresses in code (preliminary text-pass)
//-----------------------------------------------------------------------------

void fill_labels (label *labels, line_t *line);

//-----------------------------------------------------------------------------
//! Analyses the command in input-line
//! @param [in]  asm_buffer   - the massive of binary code, where we store compiled code
//! @param [in]  labels       - the massive of binary code, where we put all labels' addresses
//! @param [in]  bytes_buffer - the number of bytes in buffer
//! @param [in]  line         - the line of command
//! @param [in]  res          - the stream of result-file
//! @param [in]  file_listing - the stream of file_listing-file
//! @param [out] error_state  - can be ERROR_FORMAT, ERROR_UNKNOWN_CMD, OK
//! @version 3.0
//! @authors Vokerlee
//! @brief Prints bytes in buffer, based on the commands and numbers in line, writes file_listing
//-----------------------------------------------------------------------------

int process_line (char *asm_buffer, label *labels, int *bytes_buffer, line_t *line, FILE *file_listing);

//-----------------------------------------------------------------------------
//! Retrieves of necessary values in line
//! @param [in]  line        - the line of command
//! @param [in]  command     - the name of command, where we put the word of command from line
//! @param [in]  label_name  - the name of label, where we put the name of label if the line is label
//! @param [in]  number      - the number in line, where we put the value of command from line
//! @param [in]  regist      - the number of register of nCPU
//! @param [out] error_state - is equal to OK, if there are errors is equal to some error_number
//! @version 2.0
//! @authors Vokerlee
//! @brief Retrieves of necessary values in line: command, regist or number, if they present
//-----------------------------------------------------------------------------

int read_command (line_t *line, char *command, char *label_name, char *regist, double *number);

//-----------------------------------------------------------------------------
//! Puts addresses of labels in struct label and prints file_listing
//! @param [in] labels       - the massive of binary code, where we put all labels' addresses
//! @param [in] bytes_buffer - the number of bytes in buffer
//! @param [in] file_listing - the stream of file_listing-file
//! @version 2.0
//! @authors Vokerlee
//! @brief Puts addresses of labels in struct label and prints file_listing
//-----------------------------------------------------------------------------

void process_label_line (label *labels, int *bytes_buffer, FILE *file_listing);

//-----------------------------------------------------------------------------
//! Encodes commands, which use labels and prints file_listing
//! @param [in] asm_buffer     - the massive of binary code, where we put all assembler info
//! @param [in] bytes_buffer   - the number of bytes in buffer
//! @param [in] command        - the name of command, where we put the word of command from line
//! @param [in] label_name     - the name of label, where we put the name of label if the line is label
//! @param [in] labels         - the massive of binary code, where we put all labels' addresses
//! @param [in] command_number - the number, where we put the argument of command
//! @param [in] file_listing   - the stream of file_listing-file
//! @version 2.0
//! @authors Vokerlee
//! @brief Encodes commands, which use labels and prints file_listing
//-----------------------------------------------------------------------------

void process_label_cmd_line (char *asm_buffer, int *bytes_buffer, char *command, char *label_name, label *labels, int command_number, FILE *file_listing);

//-----------------------------------------------------------------------------
//! Encodes commands without arguments and prints file_listing
//! @param [in] asm_buffer     - the massive of binary code, where we put all assembler info
//! @param [in] command        - the name of command, where we put the word of command from line
//! @param [in] bytes_buffer   - the number of bytes in buffer
//! @param [in] command_number - the number, where we put the argument of command
//! @param [in] file_listing   - the stream of file_listing-file
//! @version 2.0
//! @authors Vokerlee
//! @brief Encodes commands without arguments and prints file_listing
//-----------------------------------------------------------------------------

void process_no_argument_cmd_line (char *asm_buffer, int *bytes_buffer, char *command, int command_number, int regist, FILE *file_listing);

//-----------------------------------------------------------------------------
//! Encodes commands with one argument and prints file_listing
//! @param [in] buffer         - the massive of binary code, where we put all assembler info
//! @param [in] command        - the name of command, where we put the word of command from line
//! @param [in] bytes_buffer   - the number of bytes in buffer
//! @param [in] command_number - the number of command in hash_search
//! @param [in] number         - the number, where we put the argument of command
//! @param [in] file_listing   - the stream of file_listing-file
//! @version 2.0
//! @authors Vokerlee
//! @brief Encodes commands with one argument and prints file_listing
//-----------------------------------------------------------------------------

void process_argument_cmd_line (char *asm_buffer, int *bytes_buffer, char *command, int command_number, double number, FILE *file_listing);

//-----------------------------------------------------------------------------
//! Encodes commands, connected with RAM and prints file_listing
//! @param [in] buffer         - the massive of binary code, where we put all assembler info
//! @param [in] command        - the name of command, where we put the word of command from line
//! @param [in] bytes_buffer   - the number of bytes in buffer
//! @param [in] command_number - the number of command in hash_search
//! @param [in] number         - the number, where we put the argument of command
//! @param [in] regist         - the number of register of nCPU
//! @param [in] file_listing   - the stream of file_listing-file
//! @version 2.0
//! @authors Vokerlee
//! @brief Encodes commands, connected with RAM and prints file_listing
//-----------------------------------------------------------------------------

void process_ram_line (char *asm_buffer, int *bytes_buffer, char *command, int command_number, double number, char regist, FILE *file_listing);

//-----------------------------------------------------------------------------
//! Writes ID of program in compiled code
//! @param [in] res - the stream of file, where we put ID
//! @version 1.0
//! @authors Vokerlee
//! @brief Writes ID of program in compiled code
//-----------------------------------------------------------------------------

void write_id_asm (FILE *res);

//-----------------------------------------------------------------------------
//! Calculates the hash number
//! @param [in]  line - the line of command
//! @param [out] hash - the number of hash of the command in line
//! @version 1.0
//! @authors Vokerlee
//! @brief Calculates the hash number of command in line
//-----------------------------------------------------------------------------

int hash_text (const char *line);

//-----------------------------------------------------------------------------
//! Searches of hash of command in list of hashes
//! @param [in] hash_command - the hash of command
//! @param [out] position    - the position of element in hash_data massive
//! @version 1.0
//! @authors Vokerlee
//! @brief Searches the number of command, matching its hash
//-----------------------------------------------------------------------------

int hash_search (int hash_command);

//-----------------------------------------------------------------------------
//! Writes hex-code of given memory part
//! @param [in] hex      - the pointer to given memory part
//! @param [in] size_hex - the volume of memory part
//! @param [in] res      - the stream of file, where the result will be writtenb
//! @version 1.0
//! @authors Vokerlee
//! @brief Writes hex-code of given memory part
//-----------------------------------------------------------------------------

void hex_print (unsigned char *hex, size_t size_hex, FILE *res);


#endif // COMPILE_H_INCLUDED
