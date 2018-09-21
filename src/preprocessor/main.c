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
 * Read up to <size> characters from <fp> into <buff>,
 * until the buffer is filled, or a preprocessor command has been encountered.
 *
 * Return zero if EOF, else non-zero
 */
int read(FILE* fp, char* buff, int size)
{
  regex_t regex;
  int reti;

  fgets(buff, size, fp);
  
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

  int size = 2047;
  char buff[size + 1];

  while (!read(fp, buff, size))
  {
    //printf("%s", buff);
  }
  fclose(fp);

  return 0;
}
