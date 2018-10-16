#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>

#define uint64 uint64_t
#define uint32 uint32_t
#define uint16 uint16_t
#define uint8  uint8_t
#define real64 double
#define real32 float

#ifndef DRUID_DEBUG
#define DRUID_DEBUG 1
#endif // DRUID_DEBUG

#if _WIN32 || __MINGW32__
#ifndef dbgPrint
#if DRUID_DEBUG 
#define dbgPrint(_str, ...) printf(_str, __VA_ARGS__)
#else
#define dbgPrint(x) // dbgPrint(x)
#endif // DRUID_DEBUG
#endif // dbgPrint
#endif // WIN32 || __MINGW32__

#if __linux__
#ifndef dbgPrint
#if DRUID_DEBUG 
#define dbgPrint(_str, ...) printf(_str, ##__VA_ARGS__)
#else
#define dbgPrint(x) // dbgPrint(x)
#endif // DRUID_DEBUG
#endif // dbgPrint
#endif // __linux__
// [ assemblyDruid ] osx version of above macro; use #if __APPLE__

#ifndef quick_del
#define quick_del(x) if(x) { free(x); }
#endif // quick_del

typedef enum
    {
        false = 0,
        true =  1
    } bool;

typedef enum
    {
        directive_none,
        directive_summon,
        directive_custom
    } PreprocessingDirective;

// [ assemblyDruid::TODO ] wrote a lot of code in main() that should be constructing these.
// Either construct them as intended, or remove this if it is unneeded.

typedef struct
{
    PreprocessingDirective directive;
    char* macro_definition;
} MacroCache;
MacroCache MacroCacheStream[100];

typedef struct
{
    char* key;
    char* value;

    ssize_t key_buffer_len;
    ssize_t value_buffer_len;
} char_char_dict;

typedef struct
{
    struct temporary_file_information
    {
        char*   file_buffer;
        ssize_t total_file_size;
        FILE*   file_stream;
    } temp;

    char_char_dict** tome_entries;

    ssize_t tome_entries_buffer_len; // allocation/reallocation
    ssize_t num_tome_entries; // iteration
} TOME;

#ifndef mlstring // multi_line_string
#define mlstring(x) (#x)
#endif // mlstring

#ifndef TokenStreamSize
#define TokenStreamSize (sizeof(TokenStream)/sizeof(TokenStream[0]))
#endif // TokenStreamSize

typedef enum
    {
	terminal_type_none, // [ cfarvin::TODO ] remove this? probably.
	terminal_type_unknown,
	terminal_type_terminal,
	terminal_type_nonterminal
    } TerminalType;

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
