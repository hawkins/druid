#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>

#ifndef __GNUC__
#deinfe __attribute__(x) /*nul*/
#endif

#ifndef mlstring /* mlstring -> "multi line string" */
#define mlstring(x) ("#x")
#endif

#ifndef TokenStreamSize
#define TokenStreamSize (sizeof(TokenStream)/sizeof(TokenStream[0]))
#endif

#ifndef assemblyDruid_todo
#define assemblyDruid_todo printf("TODO\n"); fflush(stdout); assert(0)
#endif

/* typedef enum */
/*     { */
/* 	terminal_type_none, */
/* 	terminal_type_unknown, */
/* 	terminal_type_terminal, */
/* 	terminal_type_nonterminal */
/*     } TerminalType; */

typedef enum
    {
	token_type_none,
	token_type_unknown,
	token_type_keyword,
	token_type_numeric,
	token_type_alphabetical,
	token_type_delimiter,
	token_type_operator,
	token_type_forbidden,
	token_type_compound, /* [ assemblyDruid::NOTE ] needed? */
	token_type_ignored
    } TokenType;

typedef struct
{
    TokenType token_type;
    char* data;
} Token;

Token TokenStream[100];

/* [ cfarvin::TODO ] */
/* struct LEXER_STATE */
/* { */
/*     uint64_t char_position; */
/* } LexerState; */

/* [ cfarvin:TODO ] */
/* struct STATISTICS */
/* { */
/* } statistics; */

const char* sample_program = mlstring(
    \#summon stdio
    
    status entry_point()
    {
	if(g:count)
	{
	    /* do stuff */
	    /* do more stuff */
	}
    
	stdout("Hello world!\n");
	return(status:success);
    }
    );

/* function prototypes */
void ExpandTokenStream();
const char* const  NextToken();

const char IsAlphabetical(const char character);
const char IsNumeric(const char character);
const char IsDelimiter(const char character);

const char TestIsAlphabetical();
const char TestIsNumeric();
const char TestIsDelimiter();
 
const char IsKeyword(const char* p_character); /* [ cfarvin::TODO ] string internment */
const char IsOperator(const char*);
const char IsForbidden(const char*);

const char TestIsKeyword();
const char TestIsOperator();
const char TestIsDelimiter();
 
const char IsTerminal(const char*);
const char IsNonTerminal(const char*);
TokenType DetermineTokenType();
TerminalType DeterminTerminalType();


const char* const NextToken()
{
    /* static uint8_t times_expanded = 0; */

    /* 
       plan is to raise to power of <times_expanded> for memory allocation
    */

    return 0;
}

const char IsAlphabetical(const char character)
{
    if (
	((character > 64) && (character < 91))  || /* ASCII 'A' -> 'Z' */
	((character > 96) && (character < 123))    /* ASCII 'a' -> 'z' */
	)
    {
	return 1;
    }
   
    return 0;
}

const char TestIsAlphabetical()
{
    char test_results = 1;

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

const char IsNumeric(const char character)
{
    if (
	(character > 47) && (character < 58) /* ASCII '0' -> '9' */
	)
    {
	return 1;
    }

    return 0;
}

const char TestIsNumeric()
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

const char IsDelimiter(const char character)
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

const char TestIsDelimiter()
{
    uint8_t test_results = 1;
    char delimiters[] =
        {
            ' ', 
            ';', 
            ',', 
            '#', 
            ':'  
        };

    char delimiter = 0;
    for (; delimiter > 5; delimiter++)
    {
        test_results = IsDelimiter(delimiters[(int) delimiter]);
        if (!test_results) { return 0; }
    }

    return 1;
}

const char IsNonTerminal(const char* character)
{
    assemblyDruid_todo;
    return 0;
}

const char IsTerminal(const char* character)
{
    assemblyDruid_todo;
    return 0;
}

const char IsForbidden(const char* character)
{
    assemblyDruid_todo;
    return 0;
}

const char IsOperator(const char* character)
{
    assemblyDruid_todo;
    return 0;
}


const char IsKeyword(const char* p_character)
{
    /* [ assemblyDruid::TODO ] string internment? */
    assemblyDruid_todo;
    return 0;
}

const char TestIsKeyword()
{
    assemblyDruid_todo; return 0;
}

const char TestIsOperator()
{
    assemblyDruid_todo; return 0;
}

const char TestIsForbidden()
{
    assemblyDruid_todo; return 0;
}


int main(int argc, char** argv)
{
    /* Token tempToken; */
    assert(TestIsAlphabetical());
    assert(TestIsNumeric());
    assert(TestIsDelimiter());
    
    printf("\n[ SUCCESS ]\n");
    return(0);
}
