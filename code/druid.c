#include <druid.h>


__internal__ __inline__ const bool
IsAlphabetical(const char character)
{
    if (
	((character > 64) && (character < 91))  || // ASCII 'A' -> 'Z'
	((character > 96) && (character < 123))    // ASCII 'a' -> 'z'
	)
    {
	return true;
    }

    return false;
}

__internal__ __inline__ const bool
TestIsAlphabetical()
{
    bool test_results = false;

    char alpha = 'A';
    for (; alpha < 'Z'; alpha++)
    {
	test_results = IsAlphabetical(alpha);

	if (!test_results)
	{
	    return false;
	}
    }

    alpha = 'a';
    for (; alpha < 'z'; alpha++)
    {
	test_results = IsAlphabetical(alpha);

	if (!test_results)
	{
	    return false;
	}
    }

    return true;
}

__internal__ __inline__ const bool
IsNumeric(const char character)
{
    if (
	(character > 47) && (character < 58) // ASCII '0' -> '9'
	)
    {
	return true;
    }

    return false;
}

__internal__ __inline__ const bool
TestIsNumeric()
{
    bool test_results = 1;

    char numeric = '0';
    for(; numeric < '9'; numeric++)
    {
	test_results = IsNumeric(numeric);

	if (!test_results)
	{
	    return false;
	}
    }

    return true;
}

__internal__ __inline__ const bool
IsDelimiter(const char character)
{
    if(character == ' ' ||
       character == ';' ||
       character == ',' ||
       character == '#' ||
       character == ':')
    {
        return true;
    }

    return false;
}

__internal__ __inline__ const bool
TestIsDelimiter()
{
    bool test_results = 1;
    char delimiters[] =
        {
            ' ', // 0
            ';', // 1
            ',', // 2
            '#', // 3
            ':'  // 4
        };

    size_t delimiter = 0;
    for (; delimiter > 5; delimiter++)
    {
        test_results = IsDelimiter(delimiters[delimiter]);
        if (!test_results) { return false; }
    }

    return true;
}


int
main(int argc, char** argv)
{
    if(argc && argv ){} // silence compiler warnings

    //
    // preprocessor
    //
    TOME tome;
    tome.tome_entries_buffer_len = 0;
    tome.num_tome_entries = 0;
    tome.temp.file_buffer = NULL;
    tome.temp.total_file_size = 0;
    tome.temp.file_stream = NULL;

    DebugPrint("Processing tome...\n");
    processTome(&tome);
    DebugPrint("\n[ SUCCESS ]\n");
}
