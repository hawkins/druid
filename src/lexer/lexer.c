#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __GNUC__
#deinfe __attribute__(x) /*nul*/
#endif

#ifndef mlstring /* mlstring -> "multi line string" */
#define mlstring(x) ("#x")
#endif

#ifndef TokenStreamSize
#define TokenStreamSize (sizeof(TokenStream) / sizeof(TokenStream[0]))
#endif

#ifndef assemblyDruid_todo
#define assemblyDruid_todo \
  printf("TODO\n");        \
  fflush(stdout);          \
  assert(0)
#endif

typedef enum {
  /* Feedback */
  tok_none,
  /* Punctuation */
  // tok_dot,
  tok_semicolon,
  tok_colon,
  tok_comment_multiline,
  tok_comment,
  tok_brace_open,
  tok_brace_close,
  tok_paren_open,
  tok_paren_close,
  tok_string,
  tok_char,
  // tok_quote_double, // TODO: Can't we just have string/char instead?
  // tok_quote_single,
  /* Operators */
  tok_equal,
  tok_equal_double,
  tok_minus,
  tok_plus,
  tok_asterisk,
  tok_slash,
  /* Keywords */
  tok_return,
  /* User definitions */
  tok_identifier,
} TokenType;

typedef struct {
  TokenType token_type;
  char* data;
} Token;

Token* TokenStream[100];

/* function prototypes */
void ExpandTokenStream();
void NextToken(char*);

const char IsAlphabetical(const char character);
const char IsNumeric(const char character);
const char IsDelimiter(const char character);

const char TestIsAlphabetical();

const char IsKeyword(
    const char* p_character); /* [ cfarvin::TODO ] string internment */
const char IsOperator(const char*);
const char IsForbidden(const char*);

const char IsTerminal(const char*);
const char IsNonTerminal(const char*);
TokenType DetermineTokenType();

void NextToken(char* buffer) {
  fscanf(stdin, "%s", buffer);
  // Read characters until a token type can be determined by pattern matching
  // Read characters until a new character is read that does not match the
  // current pattern Set buffer to the read token
}

const char IsAlphabetical(const char character) {
  if (((character > 64) && (character < 91)) || /* ASCII 'A' -> 'Z' */
      ((character > 96) && (character < 123))   /* ASCII 'a' -> 'z' */
  ) {
    return 1;
  }

  return 0;
}

const char TestIsAlphabetical() {
  char test_results = 1;

  char alpha = 'A';
  for (; alpha < 'Z'; alpha++) {
    test_results = IsAlphabetical(alpha);

    if (!test_results) {
      return 0;
    }
  }

  alpha = 'a';
  for (; alpha < 'z'; alpha++) {
    test_results = IsAlphabetical(alpha);

    if (!test_results) {
      return 0;
    }
  }

  return 1;
}

const char IsNumeric(const char character) {
  if ((character > 47) && (character < 58) /* ASCII '0' -> '9' */
  ) {
    return 1;
  }

  return 0;
}

const char TestIsNumeric() {
  uint8_t test_results = 1;

  char numeric = '0';
  for (; numeric < '9'; numeric++) {
    test_results = IsNumeric(numeric);

    if (!test_results) {
      return 0;
    }
  }

  return 1;
}

const char IsDelimiter(const char character) {
  if (character == ' ' || character == ';' || character == ',' ||
      character == '#' || character == ':') {
    return 1;
  }

  return 0;
}

const char TestIsDelimiter() {
  uint8_t test_results = 1;
  char delimiters[] = {' ', ';', ',', '#', ':'};

  char delimiter = 0;
  for (; delimiter > 5; delimiter++) {
    test_results = IsDelimiter(delimiters[(int)delimiter]);
    if (!test_results) {
      return 0;
    }
  }

  return 1;
}

const char IsNonTerminal(const char* character) {
  assemblyDruid_todo;
  return 0;
}

const char IsTerminal(const char* character) {
  assemblyDruid_todo;
  return 0;
}

const char IsForbidden(const char* character) {
  assemblyDruid_todo;
  return 0;
}

const char IsOperator(const char* character) {
  assemblyDruid_todo;
  return 0;
}

const char IsKeyword(const char* p_character) {
  /* [ assemblyDruid::TODO ] string internment? */
  assemblyDruid_todo;
  return 0;
}

const char TestIsKeyword() {
  assemblyDruid_todo;
  return 0;
}

const char TestIsOperator() {
  assemblyDruid_todo;
  return 0;
}

const char TestIsForbidden() {
  assemblyDruid_todo;
  return 0;
}

int main(int argc, char** argv) {
  /* Token tempToken; */
  assert(TestIsAlphabetical());
  assert(TestIsNumeric());
  assert(TestIsDelimiter());

  // Read in a series of characterrs separated by spaces
  // /* comment */     ["/*", "comment", "*/"]
  int number_of_tokens = 0;
  while (!feof(stdin)) {
    char* buffer = (char*)calloc((size_t)64, sizeof(char));
    // TODO: Split on delimiters, too, not just spaces
    NextToken(buffer);

    // TODO: Create a token for the buffer
    Token* tok = (Token*)malloc(sizeof(Token*));
    tok->data = buffer;
    // tok->token_type = GetTokenType(buffer);

    TokenStream[number_of_tokens] = tok;
    ++number_of_tokens;
  }

  int index;
  for (index = 0; index < number_of_tokens; ++index)
    printf("%s\n", TokenStream[index]->data);

  printf("\n[ SUCCESS ]\n");
  return (0);
}
