#ifndef __PREPROCESSOR_TOOLS__
#define __PREPROCESSOR_TOOLS__

#include <macro_tools.h>
#include <debug_tools.h>
#include <type_tools.h>


typedef enum
{
    directive_none,
    directive_summon,
    directive_custom
} PreprocessingDirective;


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

    size_t key_buffer_len;
    size_t value_buffer_len;
} char_char_dict;


typedef struct
{
    struct temporary_file_information
    {
        char*   file_buffer;
        size_t  total_file_size;
        FILE*   file_stream;
    } temp;

    char_char_dict** tome_entries;

    size_t tome_entries_buffer_len; // allocation/reallocation
    size_t num_tome_entries;        // iteration
} TOME;


// Returns true until there is nothing left to preprocess
__internal__ __inline__ bool
preprocess(TOME* const tome)
{
    if (tome)
    {

    }

    //  Preprocesing assumptions (for now):
    //  1) A preprocessing directive takes up exactly one line, unless escaped (like C/C++).
    //  [ cfarvin::NOTE ] it would be kind of neat to have #s# #e# for "start" and "end",
    //  (or whatever syntax) eliminating the need for littering the directives with / marks.
    //  1.2) It follows that each directive will end with "\n"
    //   1. Deterimine what kind of directive we are dealing with. Right now, this is just "summon"
    //   2. Pass this directive to "apply()" so that magical stuff can happen.

    return false;
}

// [ cfarvin::TODO ] Remove after use in Druid
#if __GNUC__
#pragma GCC diagnostic ignored "-Wunused-function"
#endif // __GNUC__
__internal__ __inline__ void
addTomeEntry(TOME* const tome, char_char_dict* const entry)
{
    if (tome)
    {

    }

    if (entry)
    {

    }
}
// [ cfarvin::TODO ] Remove after use in Druid
#if __GNUC__
#pragma GCC diagnostic pop
#endif // __GNUC__


#endif // __PREPROCESSOR_TOOLS__
