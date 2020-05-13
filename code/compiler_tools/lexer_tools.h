#ifndef __LEXER_TOOLS__
#define __LEXER_TOOLS__

#include <character_tools.h>
#include <operator_tools.h>


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

Token* NextToken(FILE* f) {
    char* buffer = (char*)calloc((size_t)100, sizeof(char));
    int i = 0;

    Token* result = new_Token("", tok_none);
    result->data = buffer;

    // Handle all single-character tokens first
    do {
        buffer[i] = (char)fgetc(f);
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
    } /* else if (buffer[0] == '=') {
         result->type = tok_equal;
         } */
    else if (buffer[0] == '-') {
        result->type = tok_minus;
    } else if (buffer[0] == '+') {
        result->type = tok_plus;
    } else if (buffer[0] == '*') {
        result->type = tok_asterisk;
    }
    // Note we can't return tok_slash yet because it could be a comment
    // Similarly, tok_equal could be tok_equal_double

    // Handle possible early escape
    if (result->type != tok_none) {
        // result->data = buffer;
        return result;
    }

    // Continue to read characters until a match is found
    ++i;
    do {
        do {
            buffer[i] = (char)fgetc(f);
        } while (IsWhitespace(buffer[i]));

        // Equality
        if (buffer[0] == '=') {
            if (buffer[1] == '=') {
                result->type = tok_equal_double;
            } else {
                result->type = tok_equal;

                ungetc(buffer[1], f);
                buffer[1] = '\0';
            }
            return result;
        }

        // Numbers
        if (IsNumeric(buffer[0])) {
            result->type = tok_integer;

            while (IsNumeric(buffer[i])) {
                buffer[++i] = (char)fgetc(f);
            }
            if (buffer[i] == '.') {
                result->type = tok_float;
                while (IsNumeric(buffer[i])) {
                    buffer[++i] = (char)fgetc(f);
                }
            }
            ungetc(buffer[i], f);
            buffer[i] = '\0';
            return result;
        }

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

                    buffer[i] = (char)fgetc(f);
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

                    buffer[i] = (char)fgetc(f);
                } while (buffer[i] != '\n' && buffer[i] != EOF);
                buffer[i] = '\0';

                result->type = tok_comment;
                return result;
            } else {
                // This was a tok_slash and not a comment
                ungetc(buffer[i], f);
                buffer[i] = '\0';
                result->type = tok_slash;
                return result;
            }
        }

        // Strings
        if (buffer[0] == '\"') {
            result->type = tok_string;

            // TODO: I'm pretty sure this escape logic is flawed, fix it
            do {
                buffer[++i] = (char)fgetc(f);
            } while (buffer[i] != EOF &&
                     (buffer[i - 1] != '\\' || buffer[i] != '\"'));
            if (buffer[i] == EOF) {
                buffer[i] = '\0';
            }

            return result;
        }

        // Characters
        if (buffer[0] == '\'') {
            if (buffer[1] == '\\') {
                buffer[++i] = (char)fgetc(f);
            }
            buffer[++i] = (char)fgetc(f);

            if (buffer[i] == '\'') {
                result->type = tok_char;
                return result;
            } else {
                result->type = tok_none;
                return result;
            }
        }

        // Identifier
        // [a-Z]([a-Z][0-9][_])*
        if (IsAlphabetical(buffer[0])) {
            while (IsAlphabetical(buffer[i]) || IsNumeric(buffer[i]) ||
                   buffer[i] == '_') {
                buffer[++i] = (char)fgetc(f);
            }
            ungetc(buffer[i], f);
            buffer[i] = '\0';

            result->type = tok_identifier;
            return result;
        }
        ++i;
    } while (i < 100);

    // TODO: Debug,remove me
    printf("NO TOKEN MATCHED! Buffer: %s\n", buffer);

#ifdef _WIN64
#pragma warning( push )
#pragma warning( disable : 4996)
#endif // _WIN64

    int fscanf_result = fscanf(f, "%s", buffer);

#ifdef _WIN64
#pragma warning( pop )
#endif // _WIN64

    if (fscanf_result == 0)
    {
        DebugPrint("[ FATAL ] fscanf reports zero matches\n.");
        return NULL;
    }

    result->data = buffer;
    return result;
}


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

#define EXPECT_EQUAL_STR(A, B)                                          \
    expect_str(A, TO_EQUAL, B);                                         \
    if (expect_str(A, TO_EQUAL, B) != 0)                                \
        printf("  * expected:\t%s\t(" #B ")\n    actual:\t%s\t(" #A ")\n", B, A);
#define EXPECT_EQUAL_INT(A, B)                                          \
    expect_int(A, TO_EQUAL, B);                                         \
    if (expect_int(A, TO_EQUAL, B) != 0)                                \
        printf("  * expected:\t%i\t(" #B ")\n    actual:\t%i\t(" #A ")\n", B, A);


#endif // __LEXER_TOOLS__
