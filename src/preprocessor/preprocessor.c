/*
 * Druid Pre-processor
 *
 * By Josh Hawkins and Cameron Farvin
 */
#include <stdlib.h>
#include <regex.h>
#include <string.h>

#include "preprocessor.h"

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
  strncpy(&tempfilename, filename, sizeof(filename));
  strncat(&tempfilename, ".ddp", sizeof(filename));
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
  int buffersize = 10;
  char buffer[buffersize];
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

void apply(FILE* fp, long int offset, const char* directive, const char* parameter)
{
  char filename[1024];
  strncpy(&filename, parameter, sizeof(filename));
  strncat(&filename, ".druid", sizeof(filename));

  if (!strncmp(directive, "summon", 6))
  {
    summon(fp, offset, filename);
  }
  else
  {
    fprintf(stderr, "Error: unknown preprocessor directive!");
    exit(1);
  }
}

int read(FILE* fp)
{
  regex_t regex;
  int reti;
  size_t length = 0;
  char* buffer = 0;
  
  long int offset = ftell(fp);

  if (getline(&buffer, &length, fp) == (ssize_t) 0)
    return 0;
  
  reti = regcomp(&regex, "^#\\(.*\\) \\(..*\\)$", 0);
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
