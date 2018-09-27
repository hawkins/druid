/*
 * Druid Pre-processor
 *
 * By Josh Hawkins and Cameron Farvin
 */
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

typedef enum
    {
        directive_none,
        directive_summon,
        directive_custom
    } PreprocessingDirective;

typedef struct
{
    PreprocessingDirective directive;
    char* macro_name;
    char* macro_definition;
} MacroCache;

MacroCache MacroCacheStream[100];

/* void copyfile(FILE* source, const char* target_filename) */
/* { */
/*     // TODO: Error handling is for chumps */
/*     long int offset = ftell(source); */
/*     fseek(source, 0L, SEEK_SET); */
/*     FILE* target = fopen(target_filename, "w"); */
/*     char ch = '\0'; */
/*     while(ch != EOF) */
/*     { */
/*         ch = fgetc(source); */
/*         fputc(ch, target); */
/*     } */
/*     fclose(target); */
/*     fseek(source, offset, SEEK_SET);; */
/*}*/

/* void summon(FILE* fp, long int offset, const char* filename) */
/* { */
/*     // Preprocess this file before summoning it so we know we have the final version */
/*     /\* preprocess(filename); *\/ */

/*     // Make a copy of the original file so we can re-write it back in after summoned resource */
/*     char tempfilename[1024]; */
/*     strncpy((char*) &tempfilename, filename, sizeof(filename)); */
/*     strncat((char*) &tempfilename, ".ddp", sizeof(filename)); */
/*     copyfile(fp, tempfilename); */

/*     // We'll use this offset in the tempfile to find the remaining content */
/*     long int previous_offset = ftell(fp); */

/*     // Set file position to offset so we can edit in-place */
/*     fseek(fp, offset, SEEK_SET); */

/*     // Write the summoned resource into the file */
/*     FILE* input = fopen(filename, "r"); */
/*     fseek(input, 0L, SEEK_END); */
/*     long int summoned_resource_size = ftell(fp); */
/*     fseek(input, 0L, SEEK_SET); */

/*     size_t buffersize = 10; */
/*     char* buffer = (char*) calloc(buffersize, sizeof(char)); */
/*     while (fgets(buffer, sizeof(buffer), input)) { */
/*         fwrite(buffer, sizeof(char), strnlen(buffer, buffersize), fp); */
/*     } */

/*     // Write the rest of the original file back into newly modified file */
/*     FILE* temp = fopen(tempfilename, "r"); */
/*     fseek(temp, previous_offset, SEEK_SET); */
/*     char ch = getc(temp); */
/*     while (ch != EOF) */
/*     { */
/*         fputc(ch, fp); */
/*         ch = getc(temp); */
/*     } */
/*     fclose(temp); */
/*     remove(tempfilename); */

/*     // Restore file position so program is read is unaffected */
/*     fseek(fp, offset + summoned_resource_size, SEEK_SET); */

/*     fclose(input); */
/* } */

/* void apply(FILE* fp, long int offset, PreprocessingDirective directive, const char* parameter) */
/* { */

/*     /\* */
/*       1. Determine the "PreprocessingAction" that needs to happen. */
/*       2. Do that thing from number 1. */
/*     *\/ */
/*     char filename[1024]; */
/*     strncpy((char *) &filename, parameter, sizeof(filename)); */
/*     strncat((char *) &filename, ".druid", sizeof(filename)); */

/*     if (directive == directive_summon) */
/*     { */
/*         summon(fp, offset, filename); */
/*     } */
/*     else */
/*     { */
/*         fprintf(stderr, "Error: unknown preprocessor directive!"); */
/*         exit(1); */
/*     } */
/* } */


/*
  Maybe we call this repeatedly until no further directives are found?
*/
int main(int argc, char** argv)
{
    /* [ assemblyDruid::NOTE::TODO ] turn this on after basic (working) development
       if (argc > 3)
       {
       /*
       [ assemblyDruid::TODO ] what is the purpose of the 3rd argument?
       Something about printing to stdout or not. If so, we should consider
       changing the verbiage of the below "[output file]".
    */

    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }


    /*
      Preprocesing assumptions (for now):
      1) A preprocessing directive takes up exactly one line, unless escaped (like C/C++).
      [ assemblyDruid::NOTE ] it would be kind of neat to have #s# #e# for "start" and "end",
      (or whatever syntax) eliminating the need for littering the directives with / marks.
      1.2) It follows that each directive will end with "\n"
    */

    /* 
       1. Deterimine what kind of directive we are dealing with. Right now, this is just "summon"
       2. Pass this directive to "apply()" so that magical stuff can happen.
    */

    char*  file_buffer;
    size_t total_file_size;
    size_t line_position_start;
    size_t line_position_end;
    FILE*  file_stream;
    /* [ assemblyDruid::TODO ] check all possible return values for fopen_s */
    /* [ assemblyDruid::TODO ] look into more than 'r' for better opening options */
    if(!fopen_s(&file_stream, argv[1], "r" ))  
    {
        /* Find the file len, allocate appropriate quantity of memory */
        _fseeki64( file_stream, 0, SEEK_END );
        total_file_size = ftell( file_stream );
        file_buffer = (char*) calloc(total_file_size, sizeof(char));

        _fseeki64( file_stream, 0, SEEK_SET );
        fread(file_buffer, sizeof(char), total_file_size, file_stream);
        printf("\n%s\n", file_buffer);

    }
    else
    {
        /* [ assemblyDruid::TODO ] unified system for error handling */
        printf("[ FATAL ] Could not open the file. Also, you should make better error messages\n");
        assert(0);
    }

    /*


      [ assemblyDruid::TODO:REMEMBER ] Printing jibberish on #summon dataCube line


    */
    
    /*
      Begin parsing the file_buffer for '#'
    */
    size_t cursor_position = 0;
    size_t directive_length = 0;
    char* directive_source = NULL;
    for (; cursor_position < total_file_size; cursor_position++)
    {
        if (file_buffer[cursor_position] == '#')
        {
            line_position_start = cursor_position;
            for (; cursor_position < total_file_size; cursor_position++)
            {
                char newline_found = 0;
                if(file_buffer[cursor_position] == '\n' )
                {
                    newline_found = 1;
                    line_position_end = cursor_position;
                    directive_length = line_position_end - line_position_start;
                    directive_source = (char*) calloc((line_position_end - line_position_start),
                                                      sizeof(char));
                    size_t directive_cursor = 0;
                    for (; directive_cursor < directive_length; directive_cursor++ )
                    {
                        directive_source[directive_cursor] = file_buffer[line_position_start + directive_cursor];
                    }
                    printf("[ PREPROCESSING DIRECTIVE ] %s\n", directive_source);
                    break;
                }
                /* [ assemblyDruid::TODO ] handle EOF case (no newline) */
                if ((cursor_position == (total_file_size -1)) && (!newline_found))
                {
                    printf("[ FATAL ] No newline following directive\n");
                    assert(0);
                }
            }
        }
    }
    
    cursor_position = line_position_start;
    if (directive_source) { free(directive_source); }

    /*
      [ assemblyDruid::TODO ] look into setting the msvc flag for c89, if you haven't already and if possible
    */
    fclose(file_stream);
    free(file_buffer);
    return 0;  
}
