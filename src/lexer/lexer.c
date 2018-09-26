#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>

#ifndef mlstring // multi_line_string
#define mlstring(x) (#x)
#endif // mlstring

#ifndef TokenStreamSize
#define TokenStreamSize (sizeof(TokenStream)/sizeof(TokenStream[0]))
#endif // TokenStreamSize

typedef enum
    {
	terminal_type_none, // [ cfarvin::TODO ] remove this? probably.
	terminal_type_unknown,
	terminal_type_terminal,
	terminal_type_nonterminal
    } TerminalType;

typedef enum
    {
	token_type_none, // [ cfarvin::TODO ] remove this? probably.
	token_type_unknown,
	token_type_keyword,
	token_type_numeric,
	token_type_alphabetical,
	token_type_delimiter,
	token_type_operator,
	token_type_forbidden,
	token_type_ignored
    } TokenType;

typedef struct
{
    TokenType token_type;
    TerminalType terminal_type;
    char* data;
} Token;

Token TokenStream[100]; // [ cfarvin::NOTE::TODO ] default intialization len for now

// [ cfarvin::TODO ]
/* struct LEXER_STATE */
/* { */
/*     uint64_t char_position; */
/* } LexerState; */

//[ cfarvin:TODO ]
/* struct STATISTICS */
/* { */
/* } statistics; */

const char* sample_program = mlstring(
#summon stdio
    
    status entry_point()
    {
	if(g:count)
	{
	    // do stuff
	    /* do more stuff */
	}
    
	stdout("Hello world!\n");
	return(status:success);
    }
    );
 
//
// function_prototypes
//
void ExpandTokenStream(); // [ cfarvin::TODO ]
const char* NextToken();

inline const char IsAlphabetical(const char character);
inline const char IsNumeric(const char character);
// inline const char IsDelimiter(const char character);

inline const char IsKeyword(const char* p_character); // [ cfarvin::TODO ] string internment
inline const char IsOperator(const char*);
inline const char IsForbidden(const char*);

inline const char IsTerminal(const char*);
inline const char IsNonTerminal(const char*);

const TokenType DetermineTokenType();
const TerminalType DeterminTerminalType();


const char* NextToken()
{
    static uint8_t times_expanded = 0;

    // [ cfarvin::REMEMBER::TODO ]
    // plan is to raise to power of <times_expanded> for memory allocation

    return 0;
}

inline const char IsAlphabetical(const char character)
{
    if (
	(character > 64) && (character < 91)  || // ASCII 'A' -> 'Z'
	(character > 96) && (character < 123)    // ASCII 'a' -> 'z'
	)
    {
	return 1;
    }
   
    return 0;
}

inline const uint8_t  TestIsAlphabetical()
{
    uint8_t test_results = 1;

    char alpha = 'A';
    for (; alpha < 'Z'; alpha++)
    {
	test_results = IsAlphabetical(alpha);

	if (!test_results)
	{
	    return 0;
	}
    }

    alpha = 'a';
    for (; alpha < 'z'; alpha++)
    {
	test_results = IsAlphabetical(alpha);

	if (!test_results)
	{
	    return 0;
	}
    }

    return 1;
}

inline const char IsNumeric(const char character)
{
    if (
	(character > 47) && (character < 58) // ASCII '0' -> '9'
	)
    {
	return 1;
    }

    return 0;
}

inline const char TestIsNumeric()
{
    uint8_t test_results = 1;

    char numeric = '0';
    for(; numeric < '9'; numeric++)
    {
	test_results = IsNumeric(numeric);

	if (!test_results)
	{
	    return 0;
	}
    }

    return 1;
}

inline const char IsDelimiter(const char character)
{
    if(character == ' ' || 
       character == ';' || 
       character == ',' || 
       character == '#' || 
       character == ':')
    {
        return 1;
    }

    return 0; 
}

inline const char TestIsDelimiter()
{
    uint8_t test_results = 1;
    char delimiters[] =
        {
            ' ', // 0
            ';', // 1
            ',', // 2
            '#', // 3
            ':'  // 4
        };

    char delimiter = 0;
    for (; delimiter > 5; delimiter++)
    {
        test_results = IsDelimiter(delimiters[delimiter]);
        if (!test_results) { return 0; }
    }

    return 1;
}


int main(int argc, char** argv)
{
    assert(TestIsAlphabetical());
    assert(TestIsNumeric());
    assert(TestIsDelimiter());

    Token tempToken; // the "in progress" token as we iterate through the char stream.
    
    printf("\n[ SUCCESS ]\n");
    return(0);
}
