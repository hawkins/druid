/*
 * Druid Pre-processor
 *
 * By Josh Hawkins and Cameron Farvin
 */
#include <stdio.h>

/*
 * Apply directive
 */
void apply(FILE* fp, const long int offset, const char* directive, const char* parameter);

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

