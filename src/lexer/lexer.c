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
  tok_comma,              // ,
  tok_semicolon,          // ;
  tok_colon,              // :
  tok_comment_multiline,  // /* */
  tok_comment,            // //
  tok_brace_open,         // {
  tok_brace_close,        // }
  tok_paren_open,         // (
  tok_paren_close,        // )
  tok_bracket_open,       // [
  tok_bracket_close,      // ]
  tok_string,             // " "
  tok_char,               // ' '
  // tok_quote_double, // TODO: Can't we just have string/char instead?
  // tok_quote_single,
  /* Operators */
  tok_equal,         // =
  tok_equal_double,  // ==
  tok_minus,         // -
  tok_plus,          // +
  tok_asterisk,      // *
  tok_slash,         // /
  /* Keywords */
  tok_return,  // return
  /* User definitions */
  tok_identifier,  // [a-Z][a-Z0-9]
  tok_integer,     // 0
  tok_float,       // 0.0
} TokenType;

typedef struct {
  TokenType type;
  char* data;
} Token;
Token* new_Token(char* data, TokenType type) {
  Token* tok = (Token*)malloc(sizeof(Token*));
  tok->data = data;
  tok->type = type;
  return tok;
}

Token* TokenStream[100];

/* function prototypes */
void ExpandTokenStream();

const char IsAlphabetical(const char character);
const char IsNumeric(const char character);
const char IsDelimiter(const char character);
const char IsWhitespace(const char character);

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
  int i = 0;

  Token* result = new_Token("", tok_none);
  result->data = buffer;

  // Handle all single-character tokens first
  do {
    buffer[i] = fgetc(f);
  } while (IsWhitespace(buffer[i]));

  if (buffer[0] == ';') {
    result->type = tok_semicolon;
  } else if (buffer[0] == ':') {
    result->type = tok_colon;
  } else if (buffer[0] == '(') {
    result->type = tok_paren_open;
  } else if (buffer[0] == ')') {
    result->type = tok_paren_close;
  } else if (buffer[0] == '{') {
    result->type = tok_brace_open;
  } else if (buffer[0] == '}') {
    result->type = tok_brace_close;
  } else if (buffer[0] == '[') {
    result->type = tok_bracket_open;
  } else if (buffer[0] == ']') {
    result->type = tok_bracket_close;
  } else if (buffer[0] == ',') {
    result->type = tok_comma;
  } else if (buffer[0] == '=') {
    result->type = tok_equal;
  }

  // Handle possible early escape
  if (result->type != tok_none) {
    // result->data = buffer;
    return result;
  }

  // Continue to read characters until a match is found
  ++i;
  do {
    do {
      buffer[i] = fgetc(f);
    } while (IsWhitespace(buffer[i]));

    // Comments
    if (buffer[0] == '/') {
      if (buffer[1] == '*') {
        // Read until */ is found
        do {
          ++i;

          if (i == 100) {
            // Abandon with bad token
            // result->data = buffer;
            return result;
          }

          buffer[i] = fgetc(f);
        } while (buffer[i - 1] != '*' || buffer[i] != '/');

        result->type = tok_comment_multiline;
        return result;
      } else if (buffer[1] == '/') {
        // Read until end of line is found
        do {
          ++i;

          if (i == 100) {
            // Abandon with bad token
            // result->data = buffer;
            return result;
          }

          buffer[i] = fgetc(f);
        } while (buffer[i] != '\n' && buffer[i] != EOF);
        buffer[i] = '\0';

        result->type = tok_comment;
        return result;
      }
    }

    // Identifier
    // [a-Z]([a-Z][0-9][_])*
    if (IsAlphabetical(buffer[0])) {
      // TODO: Correct this logic to fit above pattern
      while (IsAlphabetical(buffer[i]) || IsNumeric(buffer[i]) ||
             buffer[i] == '_') {
        buffer[++i] = fgetc(f);
      }
      ungetc(buffer[i], f);
      buffer[i] = '\0';

      result->type = tok_identifier;
      return result;
    }
    ++i;
  } while (i < 100);

  // TODO: Debug,remove me
  fscanf(f, "%s", buffer);
  result->data = buffer;
  return result;

  // TODO:
  //  Read characters until a token type can be determined by pattern matching
  //  Read characters until a new character is read that does not match the
  //  current pattern Set buffer to the read token
}

#ifdef _TEST
uint64_t test_NextToken_case(char* inputs, Token* expecteds[], int length) {
  printf("  case: %s\n", inputs);

  FILE* fp = fmemopen(inputs, strlen(inputs), "r");
  setbuf(fp, inputs);

  int errors = 0;
  int i;
  for (i = 0; i < length; ++i) {
    char buffer[100];
    Token* tok = NextToken(fp);
    // TODO: EXPECT_EQUAL_TOKEN
    errors += EXPECT_EQUAL_STR(tok->data, expecteds[i]->data);
    errors += EXPECT_EQUAL_INT(tok->type, expecteds[i]->type);
  }
  fclose(fp);
  return errors;
}
uint64_t test_NextToken() {
  int case1_length = 5;
  Token* case1[case1_length];
  case1[0] = new_Token("status", tok_identifier);
  case1[1] = new_Token("func", tok_identifier);
  case1[2] = new_Token("(", tok_paren_open);
  case1[3] = new_Token(")", tok_paren_close);
  case1[4] = new_Token("{", tok_brace_open);
  int errors = test_NextToken_case("status func() {", case1, case1_length);

  int case2_length = 1;
  Token* case2[case2_length];
  case2[0] = new_Token(",", tok_comma);
  errors += test_NextToken_case(",", case2, case2_length);

  int case3_length = 1;
  Token* case3[case3_length];
  case3[0] = new_Token("/* test  */", tok_comment_multiline);
  errors += test_NextToken_case("/* test  */", case3, case3_length);

  int case4_length = 6;
  Token* case4[case4_length];
  case4[0] = new_Token("a", tok_identifier);
  case4[1] = new_Token("=", tok_equal);
  case4[2] = new_Token("b", tok_identifier);
  case4[3] = new_Token("(", tok_paren_open);
  case4[4] = new_Token(")", tok_paren_close);
  case4[5] = new_Token("// do", tok_comment);
  errors += test_NextToken_case("a = b() // do", case4, case4_length);
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

const char IsWhitespace(const char character) {
  if (character == ' ' || character == '\t' || character == '\n') return 1;
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
