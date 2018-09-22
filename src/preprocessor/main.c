/*
 * Druid Pre-processor
 *
 * By Josh Hawkins and Cameron Farvin
 */
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

/*
 * Apply directive
 */
void apply(FILE* fp, const int offset, const char* directive, const char* parameter)
{
  if (!strncmp(directive, "summon", 6))
  {
    printf("Found summon\n");
    // TODO: include parameter file here in-place in fp
  }
}

/*
 * Read up to <size> characters from <fp> into <buff>,
 * until the buffer is filled, or a preprocessor command has been encountered.
 *
 * Return zero if EOF, else non-zero
 */
int read(FILE* fp)
{
  regex_t regex;
  int reti;
  int offset;
  size_t length = 0;
  char* buffer = 0;
  
  if (getline(&buffer, &length, fp) == (ssize_t) 0)
    return 0;

  offset = offset + length;
  
  reti = regcomp(&regex, "^\\s*#\\(.*\\) \\(..*\\)$", 0);
  if (reti) {
    fprintf(stderr, "Failed to compile preprocessor directive regex!\n");
    exit(1);
  }
  
  regmatch_t match[length];
  reti = regexec(&regex, buffer, length, match, 0);
  if (!reti) {
    // Whole match
    char* result = (char*) malloc(match[0].rm_eo - match[0].rm_so);
    strncpy(result, &buffer[match[0].rm_so], match[0].rm_eo - match[0].rm_so - 1);
    result[match[0].rm_eo - match[0].rm_so - 1] = '\0';
    
    // Group 1 (i.e., summon)
    char* directive = (char*) malloc(match[1].rm_eo - match[1].rm_so);
    strncpy(directive, &buffer[match[1].rm_so], match[1].rm_eo - match[1].rm_so);
    
    // Group 2 (i.e., file)
    char* parameter = (char*) malloc(match[2].rm_eo - match[2].rm_so);
    strncpy(parameter, &buffer[match[2].rm_so], match[2].rm_eo - match[2].rm_so - 1);
    parameter[match[2].rm_eo - match[2].rm_so - 1] = '\0';

    printf("Found directive: %s (%s, %s)\n", result, directive, parameter);
    apply(fp, offset, directive, parameter);

    free(result);
    free(directive);
    free(parameter);
  }
  else if (reti != REG_NOMATCH) {
    char error[100];
    regerror(reti, &regex, error, sizeof(error));
    fprintf(stderr, "Regex match failed: %s\n", error);
    exit(1);
  }
  regfree(&regex);
  free(buffer);

  return feof(fp);
}

int main(int argc, char** argv)
{
  if (argc > 3)
    printf("Too many arguments!\n");
  if (argc < 2 || argc > 3)
  {
    printf("DPP: <input file> [output file]\n");
    return 0;
  }

  // Read and preprocess whole file line-by-line
  FILE* fp = fopen(argv[1], "r+");
  while (!read(fp)) {}
  fclose(fp);

  return 0;
}
