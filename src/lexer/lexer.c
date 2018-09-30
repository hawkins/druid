#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __GNUC__
#define __attribute__(x) /*nul*/
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

#ifdef _TEST
enum COMPARISON_OPERATOR {
  TO_EQUAL,
  TO_NOT_EQUAL,
  TO_BE_GREATER_THAN,
  TO_BE_LESS_THAN,
  TO_BE_GREATER_THAN_OR_EQUAL_TO,
  TO_BE_LESS_THAN_OR_EQUAL_TO
};
uint8_t expect_str(char* actual, enum COMPARISON_OPERATOR c, char* expected) {
  switch (c) {
    case TO_EQUAL:
      if (strcmp(actual, expected) != 0) return 1;
      break;
    case TO_NOT_EQUAL:
      if (strcmp(actual, expected) == 0) return 1;
      break;
    case TO_BE_GREATER_THAN:
      if (strcmp(actual, expected) <= 0) return 1;  // TODO: Check this
      break;
    case TO_BE_LESS_THAN:
      if (strcmp(actual, expected) >= 0) return 1;  // TODO: Check this
      break;
    case TO_BE_GREATER_THAN_OR_EQUAL_TO:
      if (strcmp(actual, expected) < 0) return 1;
      break;
    case TO_BE_LESS_THAN_OR_EQUAL_TO:
      if (strcmp(actual, expected) > 0) return 1;
      break;
    default:
      return 1;
  }
  return 0;
}
uint8_t expect_int(int actual, enum COMPARISON_OPERATOR c, int expected) {
  switch (c) {
    case TO_EQUAL:
      if (actual != expected) return 1;
      break;
    case TO_NOT_EQUAL:
      if (actual == expected) return 1;
      break;
    case TO_BE_GREATER_THAN:
      if (actual <= expected) return 1;
      break;
    case TO_BE_LESS_THAN:
      if (actual >= expected) return 1;
      break;
    case TO_BE_GREATER_THAN_OR_EQUAL_TO:
      if (actual < expected) return 1;
      break;
    case TO_BE_LESS_THAN_OR_EQUAL_TO:
      if (actual > expected) return 1;
      break;
    default:
      return 1;
  }
  return 0;
}
#define EXPECT_EQUAL_STR(A, B)         \
  expect_str(A, TO_EQUAL, B);          \
  if (expect_str(A, TO_EQUAL, B) != 0) \
    printf("  * expected:\t%s\t(" #B ")\n    actual:\t%s\t(" #A ")\n", B, A);
#define EXPECT_EQUAL_INT(A, B)         \
  expect_int(A, TO_EQUAL, B);          \
  if (expect_int(A, TO_EQUAL, B) != 0) \
    printf("  * expected:\t%i\t(" #B ")\n    actual:\t%i\t(" #A ")\n", B, A);
#define TEST(FUNC)                              \
  do {                                          \
    printf(#FUNC "::\n");                       \
    int errors = test_##FUNC();                 \
    if (errors > 0)                             \
      printf("-> FAILED: %i errors\n", errors); \
    else                                        \
      printf("-> PASS\n");                      \
    printf("\n");                               \
    numFailures += errors;                      \
  } while (0)
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
  TokenType type;
  char* data;
} Token;

Token* TokenStream[100];

/* function prototypes */
void ExpandTokenStream();

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

Token* NextToken(FILE* f) {
  char* buffer = (char*)calloc((size_t)100, sizeof(char));
  fscanf(f, "%s", buffer);

  Token* result = (Token*)malloc(sizeof(Token*));
  result->data = buffer;
  result->type = tok_identifier;
  return result;

  // TODO:
  //  Read characters until a token type can be determined by pattern matching
  //  Read characters until a new character is read that does not match the
  //  current pattern Set buffer to the read token
}

#ifdef _TEST
uint64_t test_NextToken() {
  char* inputs = "status func() {";
  char* expecteds[5];
  expecteds[0] = "status";
  expecteds[1] = "func";
  expecteds[2] = "(";
  expecteds[3] = ")";
  expecteds[4] = "{";

  FILE* fp = fmemopen(inputs, strlen(inputs), "r");
  setbuf(fp, inputs);

  int length = sizeof(expecteds) / sizeof(expecteds[0]);
  int errors = 0;
  int i;
  for (i = 0; i < length; ++i) {
    char buffer[100];
    Token* tok = NextToken(fp);
    errors += EXPECT_EQUAL_STR(tok->data, expecteds[i]);
  }
  fclose(fp);
  return errors;
}
#endif

const char IsAlphabetical(const char character) {
  if (((character > 64) && (character < 91)) || /* ASCII 'A' -> 'Z' */
      ((character > 96) && (character < 123))   /* ASCII 'a' -> 'z' */
  ) {
    return 1;
  }

  return 0;
}

#ifdef _TEST
const char test_IsAlphabetical() {
  char test_results = 1;

  char alpha = 'A';
  for (; alpha < 'Z'; alpha++) {
    test_results = IsAlphabetical(alpha);

    if (!test_results) {
      return 1;
    }
  }

  alpha = 'a';
  for (; alpha < 'z'; alpha++) {
    test_results = IsAlphabetical(alpha);

    if (!test_results) {
      return 1;
    }
  }

  return 0;
}
#endif

const char IsNumeric(const char character) {
  if ((character > 47) && (character < 58) /* ASCII '0' -> '9' */
  ) {
    return 1;
  }

  return 0;
}

#ifdef _TEST
const char test_IsNumeric() {
  uint8_t test_results = 1;

  char numeric = '0';
  for (; numeric < '9'; numeric++) {
    test_results = IsNumeric(numeric);

    if (!test_results) {
      return 1;
    }
  }

  return 0;
}
#endif

const char IsDelimiter(const char character) {
  if (character == ' ' || character == ';' || character == ',' ||
      character == '#' || character == ':') {
    return 1;
  }

  return 0;
}

#ifdef _TEST
const char test_IsDelimiter() {
  uint8_t test_results = 1;
  char delimiters[] = {' ', ';', ',', '#', ':'};

  char delimiter = 1;
  for (; delimiter > 5; delimiter++) {
    test_results = EXPECT_EQUAL_INT(IsDelimiter(delimiters[(int)delimiter]), 1);
    if (!test_results) {
      return 1;
    }
  }

  return 0;
}
#endif

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
#ifdef _TEST
  /* Used in TEST macros to track total test failures */
  uint64_t numFailures = 0;

  /* Test functions */
  TEST(IsAlphabetical);
  TEST(IsNumeric);
  TEST(IsDelimiter);
  TEST(NextToken);

  /* Print final results */
  printf("\n=======================\n");
  if (numFailures > 0)
    printf("FAIL: %llu tests failed\n", numFailures);
  else
    printf("PASS: All tests succeeded! Hooray!\n");
  printf("=======================\n");
  return numFailures;
#else
  // Read in a series of characterrs separated by spaces
  // /* comment */     ["/*", "comment", "*/"]
  int number_of_tokens = 0;
  while (!feof(stdin)) {
    Token* tok = NextToken(stdin);
    TokenStream[number_of_tokens] = tok;
    ++number_of_tokens;
  }

  int index;
  for (index = 0; index < number_of_tokens; ++index)
    printf("%s\n", TokenStream[index]->data);

  printf("\n[ SUCCESS ]\n");
  return 0;
#endif
}
