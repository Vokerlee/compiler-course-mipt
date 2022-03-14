#ifndef DISASSEEMBLER_H_INCLUDED
#define DISASSEEMBLER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "enum.h"

const int FILE_LISTING_INDENT = 35;

//-----------------------------------------------------------------------------
//! Disassemblers byte-code from source to res
//! @param [in] source      - the stream of compiled_code-file
//! @param [in] res         - the stream of results-file
//! @param [in] disasm_list - the stream of disassembler-listing
//! @version 2.0
//! @authors Vokerlee
//! @brief Disassemblers byte-code from source to res, printing disasm-listing
//-----------------------------------------------------------------------------

void disasm_code (FILE *source, FILE *res, FILE *disasm_list);

//-----------------------------------------------------------------------------
//! Prints the disasm_code_listing header
//! @param [in] res - the stream of results-file
//! @version 2.0
//! @authors Vokerlee
//! @brief Prints the disasm_code_listing header
//-----------------------------------------------------------------------------

void print_disasm_list_header (FILE *res);

//-----------------------------------------------------------------------------
//! Restores asm-commands
//! @param [in] bytes       - the number of bytes of compiled_code
//! @param [in] code        - the massive of bytes of compiled_code
//! @param [in] labels      - the massive of labels
//! @param [in] res         - the stream of results-file (reconstructed asm-program)
//! @param [in] disasm_list - the stream of disassembler-listing
//! @version 2.0
//! @authors Vokerlee
//! @brief Restores asm-commands from compiled code, prints all results in disasm_code.txt
//-----------------------------------------------------------------------------

void disasm_command (int bytes, char *code, int *labels, FILE *res, FILE *disasm_list);

//-----------------------------------------------------------------------------
//! Checks the code in source-file for valid ID
//! @param [in] source - the stream of source-file
//! @version 2.0
//! @authors Vokerlee
//! @brief Checks the code in source-file for valid ID
//-----------------------------------------------------------------------------

void id_verification(FILE *source);

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


#endif // DISASSEEMBLER_H_INCLUDED
