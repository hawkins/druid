#include <druid.h>


#ifndef mlstring /* mlstring -> "multi line string" */
#define mlstring(x) ("#x")
#endif

#ifndef __GNUC__
#define __attribute__(x) /*nul*/
#endif


__internal__ __call__ bool
InitPreprocess()
{
    TOME tome;
    tome.tome_entries_buffer_len = 0;
    tome.num_tome_entries = 0;
    tome.temp.file_buffer = NULL;
    tome.temp.total_file_size = 0;
    tome.temp.file_stream = NULL;

    DebugPrint("Processing tome...\n");

    // [ cfarvin::TODO ]
    // (May 12 2020) There's also a preprocess(TOME* ) in preprocessor_tools.h
    // One of these must go.
    processTome(&tome);

    DebugPrint("\n[ SUCCESS ]\n");
    return true;
}

__internal__ __call__ bool
InitLex()
{
    // Read in a series of characterrs separated by spaces
    // /* comment */     ["/*", "comment", "*/"]

    DebugPrint("Lexing...\n");

    int number_of_tokens = 0;
    while (!feof(stdin)) {
        Token* tok = NextToken(stdin);
        TokenStream[number_of_tokens] = tok;
        ++number_of_tokens;
    }

    int index;
    for (index = 0; index < number_of_tokens - 1; ++index)
        printf("%s\n", TokenStream[index]->data);

    printf("\n[ SUCCESS ]\n");
    return true;
}


int
main(int argc, char** argv)
{
    if(argc && argv ){} // silence compiler warnings

    InitPreprocess();
    InitLex();

    return 0;
}
