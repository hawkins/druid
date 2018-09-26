/*
 * Druid Pre-processor
 *
 * By Josh Hawkins and Cameron Farvin
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

void copyfile(FILE* source, const char* target_filename)
{
    // TODO: Error handling is for chumps
    long int offset = ftell(source);
    fseek(source, 0L, SEEK_SET);
    FILE* target = fopen(target_filename, "w");
    char ch = '\0';
    while(ch != EOF)
    {
        ch = fgetc(source);
        fputc(ch, target);
    }
    fclose(target);
    fseek(source, offset, SEEK_SET);;
}

void summon(FILE* fp, long int offset, const char* filename)
{
    // Preprocess this file before summoning it so we know we have the final version
    preprocess(filename);

    // Make a copy of the original file so we can re-write it back in after summoned resource
    char tempfilename[1024];
    strncpy((char*) &tempfilename, filename, sizeof(filename));
    strncat((char*) &tempfilename, ".ddp", sizeof(filename));
    copyfile(fp, tempfilename);

    // We'll use this offset in the tempfile to find the remaining content
    long int previous_offset = ftell(fp);

    // Set file position to offset so we can edit in-place
    fseek(fp, offset, SEEK_SET);

    // Write the summoned resource into the file
    FILE* input = fopen(filename, "r");
    fseek(input, 0L, SEEK_END);
    long int summoned_resource_size = ftell(fp);
    fseek(input, 0L, SEEK_SET);

    size_t buffersize = 10;
    char* buffer = (char*) calloc(buffersize, sizeof(char));
    while (fgets(buffer, sizeof(buffer), input)) {
        fwrite(buffer, sizeof(char), strnlen(buffer, buffersize), fp);
    }

    // Write the rest of the original file back into newly modified file
    FILE* temp = fopen(tempfilename, "r");
    fseek(temp, previous_offset, SEEK_SET);
    char ch = getc(temp);
    while (ch != EOF)
    {
        fputc(ch, fp);
        ch = getc(temp);
    }
    fclose(temp);
    remove(tempfilename);

    // Restore file position so program is read is unaffected
    fseek(fp, offset + summoned_resource_size, SEEK_SET);

    fclose(input);
}

void apply(FILE* fp, long int offset, PreprocessingDirective directive, const char* parameter)
{

    /*
      1. Determine the "PreprocessingAction" that needs to happen.
      2. Do that thing from number 1.
     */
    char filename[1024];
    strncpy((char *) &filename, parameter, sizeof(filename));
    strncat((char *) &filename, ".druid", sizeof(filename));

    if (directive == preprocessing_summon)
    {
        summon(fp, offset, filename);
    }
    else
    {
        fprintf(stderr, "Error: unknown preprocessor directive!");
        exit(1);
    }
}

/*
  Preprocesing assumptions (for now):
  1) A preprocessing directive takes up exactly one line, unless escaped (like C/C++).
       [ assemblyDruid::NOTE ] it would be kind of neat to have #s# #e# for "start" and "end",
       (or whatever syntax) eliminating the need for littering the directives with / marks.
  1.2) It follows that each directive will end with "\n"
 */
int (FILE* fp)
{
    /* 
       1. Deterimine what kind of directive we are dealing with. Right now, this is just "summon"
       2. Pass this directive to "apply()" so that magical stuff can happen.
     */
    return 0;
}

void preprocess(const char* filename)
{
    FILE* fp = fopen(filename, "r+");
    while (!read(fp)) {}
    fclose(fp);
}

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
    printf("DPP: <input file> [output file]\n");
    return 0;

    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    preprocess(argv[1]);

    return 0;  
}
