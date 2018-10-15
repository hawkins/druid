#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

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

// void copyfile(FILE* source, const char* target_filename) 
// { 
//     // TODO: Error handling is for chumps 
//     long int offset = ftell(source); 
//     fseek(source, 0L, SEEK_SET); 
//     FILE* target = fopen(target_filename, "w"); 
//     char ch = '\0'; 
//     while(ch != EOF) 
//     { 
//         ch = fgetc(source); 
//         fputc(ch, target); 
//     } 
//     fclose(target); 
//     fseek(source, offset, SEEK_SET);; 
//}

// void summon(FILE* fp, long int offset, const char* filename) 
// { 
//     // Preprocess this file before summoning it so we know we have the final version 
//     /\* preprocess(filename); *\/ 

//     // Make a copy of the original file so we can re-write it back in after summoned resource 
//     char tempfilename[1024]; 
//     strncpy((char*) &tempfilename, filename, sizeof(filename)); 
//     strncat((char*) &tempfilename, ".ddp", sizeof(filename)); 
//     copyfile(fp, tempfilename); 

//     // We'll use this offset in the tempfile to find the remaining content 
//     long int previous_offset = ftell(fp); 

//     // Set file position to offset so we can edit in-place 
//     fseek(fp, offset, SEEK_SET); 

//     // Write the summoned resource into the file 
//     FILE* input = fopen(filename, "r"); 
//     fseek(input, 0L, SEEK_END); 
//     long int summoned_resource_size = ftell(fp); 
//     fseek(input, 0L, SEEK_SET); 

//     size_t buffersize = 10; 
//     char* buffer = (char*) calloc(buffersize, sizeof(char)); 
//     while (fgets(buffer, sizeof(buffer), input)) { 
//         fwrite(buffer, sizeof(char), strnlen(buffer, buffersize), fp); 
//     } 

//     // Write the rest of the original file back into newly modified file 
//     FILE* temp = fopen(tempfilename, "r"); 
//     fseek(temp, previous_offset, SEEK_SET); 
//     char ch = getc(temp); 
//     while (ch != EOF) 
//     { 
//         fputc(ch, fp); 
//         ch = getc(temp); 
//     } 
//     fclose(temp); 
//     remove(tempfilename); 

//     // Restore file position so program is read is unaffected 
//     fseek(fp, offset + summoned_resource_size, SEEK_SET); 

//     fclose(input); 
// } 

// void apply(FILE* fp, long int offset, PreprocessingDirective directive, const char* parameter) 
// { 

//     /\* 
//       1. Determine the "PreprocessingAction" that needs to happen. 
//       2. Do that thing from number 1. 
//     *\/ 
//     char filename[1024]; 
//     strncpy((char *) &filename, parameter, sizeof(filename)); 
//     strncat((char *) &filename, ".druid", sizeof(filename)); 

//     if (directive == directive_summon) 
//     { 
//         summon(fp, offset, filename); 
//     } 
//     else 
//     { 
//         fprintf(stderr, "Error: unknown preprocessor directive!"); 
//         exit(1); 
//     } 
// }

typedef struct
{
    char*  tome_file_buffer;
    size_t tome_total_file_size;
    FILE*  tome_file_stream;

    char* tome_file_name;
} TOME;
TOME tome;

void
processTome()
{
    printf("[ processTome() ]\n");
/*     // [ assemblyDruid::ASSUMPTION ] */
/*     // the name of the executable will also be the name of the .tome file */
/*     for () */
/*     { */
                            
/*     } */

/* #if _WIN32 */
/* #pragma warning(push) */
/*     // MSVC warns against <fopen> in favor of <fopen_s>. <fopen_s> is not xplatform */
/* #pragma warning( disable: 4996 ) */
/* #endif // _WIN32 */
                        
/*     tome_file_stream = fopen(, "r" ); */
    
/* #if _WIN32 */
/* #pragma warning(push) */
/* #pragma warning( disable: 4996 ) */
/* #endif // _WIN32 */
                        
/*     if (tome_file_stream != NULL) */
/*     { */
/*         // Find the file len, allocate appropriate quantity of memory */
/*         fseek(tome_file_stream, 0, SEEK_END); */
/*         tome_total_file_size = ftell( tome_file_stream ); */
/*         tome_file_buffer = (char*) calloc(tome_total_file_size, sizeof(char)); */
/*         // Reset file position, read file contents into buffer */
/*         fseek(tome_file_stream, 0, SEEK_SET); */
/*         fread(tome_file_buffer, sizeof(char), tome_total_file_size, tome_file_stream); */
/*     } */
/*     else */
/*     { */
/*         // [ assemblyDruid::TODO ] unified system for error handling */
/*         printf("[ FATAL ] Unable to locate <*.tome> file.\n"); */
/*         assert(0); */
/*     } */

                        
/*     printf("tome file:\n%s", tome_file_buffer); */
}

void
extractTomeName(const char* argv0)
{
    printf("[ extractTomeName() ] argv0: %s\n", argv0);
    
}

int
main(int argc, char** argv)
{

    /* if(!(argc >= 2)) */
    /* { */
    /*     printf("Usage: <path-to-tome>"); */
    /*     return -1; */
    /* } */
    
    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    extractTomeName(argv[0]);
    processTome();

    //  Preprocesing assumptions (for now):
    //  1) A preprocessing directive takes up exactly one line, unless escaped (like C/C++).
    //  [ assemblyDruid::NOTE ] it would be kind of neat to have #s# #e# for "start" and "end",
    //  (or whatever syntax) eliminating the need for littering the directives with / marks.
    //  1.2) It follows that each directive will end with "\n"
    //   1. Deterimine what kind of directive we are dealing with. Right now, this is just "summon"
    //   2. Pass this directive to "apply()" so that magical stuff can happen.

    char*  file_buffer;
    size_t total_file_size;
    FILE*  file_stream;
    // [ assemblyDruid::TODO ] check all possible return values for fopen
#if _WIN32
#pragma warning(push)
    // MSVC warns against <fopen> in favor of <fopen_s>. <fopen_s> is not xplatform
#pragma warning( disable: 4996 ) 
#endif // _WIN32
    
    file_stream = fopen(argv[1], "r" );
    
#if _WIN32
#pragma warning(push)
#pragma warning( disable: 4996 )    
#endif // _WIN32

    if (file_stream != NULL)
    {
        // Find the file len, allocate appropriate quantity of memory 
        fseek(file_stream, 0, SEEK_END);
        total_file_size = ftell( file_stream );
        file_buffer = (char*) calloc(total_file_size, sizeof(char));
        // Reset file position, read file contents into buffer
        fseek(file_stream, 0, SEEK_SET);
        fread(file_buffer, sizeof(char), total_file_size, file_stream);
    }
    else
    {
        // [ assemblyDruid::TODO ] unified system for error handling 
        printf("[ FATAL ] Could not open the file.\n");
        assert(0);
    }

    // Begin parsing the file_buffer for '#'
    for (size_t cursor_position = 0; cursor_position < total_file_size; cursor_position++)
    {
        size_t directive_length = 0;
        char* directive_source = NULL;
        char* directive_keyword = NULL;
        size_t line_position_start = 0;
        size_t line_position_end = 0;

        if (file_buffer[cursor_position] == '#')
        {
            // '#' was found. Find '\n'
            line_position_start = cursor_position;
            for (; cursor_position < total_file_size; cursor_position++)
            {
                char newline_found = 0;
                if(file_buffer[cursor_position] == '\n' )
                {
                    // '\n' was found. Extract directive
                    newline_found = 1;
                    line_position_end = cursor_position;

                    // [ assemblyDruid:TODO ] turn each assertion into valid error message -> exit() 
                    directive_length = line_position_end - line_position_start + 1;
                    assert(line_position_end); // dont want negative index 
                    assert(directive_length > 2);
                    directive_source = (char*) calloc(directive_length, sizeof(char));
                    size_t directive_cursor = 0;
                    for (; directive_cursor < (directive_length - 1); directive_cursor++)
                    {
                        directive_source[directive_cursor] = file_buffer[line_position_start + directive_cursor];
                    }
                    directive_source[directive_length - 1] = '\0';

                    // Directive Extracted. Determine directive type. (look for first space)
                    dbgPrint("[ PREPROCESSING DIRECTIVE SOURCE ] %s\n", directive_source);

                    assert(directive_source[0] == '#');
                    size_t directive_source_cursor = 1;
                    for (; directive_source_cursor < directive_length; directive_source_cursor++)
                    {
                        if(directive_source[directive_source_cursor] == ' ')
                        {
                            if(directive_keyword) { free(directive_keyword); }
                            directive_keyword = (char*) calloc(directive_source_cursor, sizeof(char));
                            size_t n = 0;
                            for (; n < directive_source_cursor; n++)
                            {
                                directive_keyword[n] = directive_source[n + 1];
                            }
                            directive_keyword[directive_source_cursor - 1] = '\0';
                            break;
                        }
                    }
                    dbgPrint("[ POTENTIAL DIRECTIVE TYPE ] %s\n", directive_keyword);
                    // [ assemblyDruid::NOTE::TODO ] here we get into string comparison. We can intern strings,
                    // or we can hash them. For now, for the sake of getting something working and since we can
                    // note deliberate on which would be better ( or other options in general ), i'm just going
                    // to do stuipd, expensive string comparison.
                    //
                    // This is extra extra stupid as we will have to compare between every "string" in the 
                    // list of possible directive types. (i.e. "summon" versus "if", etc). For the time being,
                    // we're only checking for "summon". We will need to have this discussion before adding more
                    // possible directive types.
                    
                    // Directive type determined. Do the thing. 
                    if (!strncmp(directive_keyword, "summon", directive_source_cursor))
                    {
                        dbgPrint("[ DIRECTIVE DETECTED ] %s, %zd characters compared\n",
                                 directive_keyword, directive_source_cursor);
                        dbgPrint("\n");
                        
                        /* void apply(FILE* fp, */
                        /*            long int offset, */
                        /*            PreprocessingDirective directive, */
                        /*            const char* parameter) */
                    }
                    break;
                }

                // [ assemblyDruid::TODO ] handle EOF case (no newline) 
                if ((cursor_position == (total_file_size -1)) && (!newline_found))
                {
                    printf("[ FATAL ] No newline following directive\n");
                    assert(0);
                }
                // cursor_position = line_position_start; 
            }
        }
        if (directive_source) { free(directive_source); }
        if (directive_keyword) { free(directive_keyword); }
    }

    // [ assemblyDruid::TODO ] look into setting the msvc flag for c99, if you haven't already and if possible
    fclose(file_stream);
    free(file_buffer);
    return 0;  
}
