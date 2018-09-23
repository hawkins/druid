/*
 * Druid Pre-processor
 *
 * By Josh Hawkins and Cameron Farvin
 */
#include <stdio.h>

/*
 * Copy a given file to a new file by the given name
 */
void copyfile(FILE* source, const char* target_filename);

/*
 * Apply directive
 */
void apply(FILE* fp, long int offset, const char* directive, const char* parameter);

/*
 * Read a line from the file and apply any preprocessor directives if found
 *
 * Return zero if EOF, else non-zero
 */
int read(FILE* fp);

/*
 * Pre-process the given file
 */
void preprocess(const char* filename);

/*
 * Include referenced file verbatim
 */
void summon(FILE* fp, long int offset, const char* filename);

