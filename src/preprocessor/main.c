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
    printf("Found summon");
  }
}

/*
 * Read up to <size> characters from <fp> into <buff>,
 * until the buffer is filled, or a preprocessor command has been encountered.
 *
 * Return zero if EOF, else non-zero
 */
int read(FILE* fp, int* offset, char* buff, int size)
{
  regex_t regex;
  int reti;

  // TODO: fgetln?
  fgetln(buff, size, fp);
  *offset = *offset + (int) strnlen(buff, size);
  
  reti = regcomp(&regex, "^\\s*#\\(.*\\) \\(..*\\)$", 0);
  if (reti) {
    fprintf(stderr, "Failed to compile preprocessor directive regex!\n");
    exit(1);
  }
  
  regmatch_t match[size + 1];
  reti = regexec(&regex, buff, size, match, 0);
  if (!reti) {
    // Whole match
    char* result = (char*) malloc(match[0].rm_eo - match[0].rm_so);
    strncpy(result, &buff[match[0].rm_so], match[0].rm_eo - match[0].rm_so - 1);
    result[match[0].rm_eo - match[0].rm_so - 1] = '\0';
    
    // Group 1 (i.e., summon)
    char* directive = (char*) malloc(match[1].rm_eo - match[1].rm_so);
    strncpy(directive, &buff[match[1].rm_so], match[1].rm_eo - match[1].rm_so);
    
    // Group 2 (i.e., file)
    char* parameter = (char*) malloc(match[2].rm_eo - match[2].rm_so);
    strncpy(parameter, &buff[match[2].rm_so], match[2].rm_eo - match[2].rm_so - 1);
    parameter[match[2].rm_eo - match[2].rm_so - 1] = '\0';

    printf("Found directive: %s (%s, %s)\n", result, directive, parameter);
    apply(fp, *offset, directive, parameter);

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

  FILE* fp = fopen(argv[1], "r+");

  int offset = 0;
  int size = 2047;
  char buff[size + 1];

  // TODO: Handle case where buffer is split in pp directive
  // ... check, from end of buffer, if we're in a pp directive
  // if so, extract start of the directive and reset buffer with
  // just this directive in the start
  while (!read(fp, &offset, buff, size))
  {
    //printf("%s", buff);
  }
  fclose(fp);

  return 0;
}
