#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include <debug_tools.h>
#include <macro_tools.h>
#include <type_tools.h>
#include <preprocessor_tools.h>


#ifndef DRUID_DEBUG
#define DRUID_DEBUG 1
#endif // DRUID_DEBUG

#ifndef mlstring // multi_line_string
#define mlstring(x) (#x)
#endif // mlstring

#ifndef TokenStreamSize
#define TokenStreamSize (sizeof(TokenStream)/sizeof(TokenStream[0]))
#endif // TokenStreamSize

// [ cfarvin::TODO ] relocate
typedef enum
{
    terminal_type_none, // [ cfarvin::TODO ] remove this? probably.
    terminal_type_unknown,
    terminal_type_terminal,
    terminal_type_nonterminal
} TerminalType;

// [ cfarvin::TODO ] relocate
typedef enum
{
    token_type_none, // [ cfarvin::TODO ] remove this? probably.
    token_type_unknown,
    token_type_keyword,
    token_type_numeric,
    token_type_alphabetical,
    token_type_delimiter,
    token_type_operator,
    token_type_forbidden,
    token_type_ignored
} TokenType;

// [ cfarvin::TODO ] relocate
typedef struct
{
    TokenType token_type;
    TerminalType terminal_type;
    char* data;
} Token;

/* void ExpandTokenStream(); // [ cfarvin::TODO ] */
/* const char* NextToken(); */
/* static inline const bool IsAlphabetical(const char character); */
/* static inline const bool IsNumeric(const char character); */
/* static inline const bool IsDelimiter(const char character); */
/* static inline const bool TestIsAlphabetical(); */
/* static inline const bool TestIsNumeric(); */
/* static inline const bool TestIsDelimiter(); */
/* static inline const bool IsKeyword(const char* p_character); // [ cfarvin::TODO ] string internment */
/* static inline const bool IsOperator(const char*); */
/* static inline const bool IsForbidden(const char*); */
/* static inline const bool IsTerminal(const char*); */
/* static inline const bool IsNonTerminal(const char*); */
const TokenType DetermineTokenType();
const TerminalType DeterminTerminalType();
Token TokenStream[100]; // [ cfarvin::NOTE::TODO ] default intialization len for now
