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
void apply(FILE* fp, const long int offset, const char* directive, const char* parameter)
{
  if (!strncmp(directive, "summon", 6))
  {
    printf("Found summon\n");
    printf(" offset: %ld\n", offset);

    // TODO: Preprocess this file before summoning it

    char filename[1024];
    strncpy(&filename, parameter, sizeof(filename));
    strncat(&filename, ".druid", sizeof(filename));
    FILE* input = fopen(filename, "r");

    // Set file position to offset so we can edit in-place
    long int previous_offset = ftell(fp);
    fseek(fp, offset, SEEK_SET);

    int buffersize = 10;
    char buffer[buffersize];
    while (fgets(buffer, sizeof(buffer), input)) {
      fwrite(buffer, sizeof(char), strnlen(buffer, buffersize), fp);
    }

    // Restore file position so program is read is unaffected
    fseek(fp, previous_offset, SEEK_SET);

    fclose(input);
  }
  else
  {
    fprintf(stderr, "Error: unknown preprocessor directive!");
    exit(1);
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
  long int offset = ftell(fp);
  size_t length = 0;
  char* buffer = 0;
  
  if (getline(&buffer, &length, fp) == (ssize_t) 0)
    return 0;
  
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

  offset = offset + length;

  return feof(fp);
}

void preprocess(const char* filename)
{
  FILE* fp = fopen(filename, "r+");
  while (!read(fp)) {}
  fclose(fp);
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

  preprocess(argv[1]);

  return 0;
}
