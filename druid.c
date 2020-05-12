#include "druid.h"

static inline const bool IsAlphabetical(const char character)
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

static inline const bool TestIsAlphabetical()
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

static inline const bool IsNumeric(const char character)
{
    if (
	(character > 47) && (character < 58) // ASCII '0' -> '9'
	)
    {
	return true;
    }

    return false;
}

static inline const bool TestIsNumeric()
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

static inline const bool IsDelimiter(const char character)
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

static inline const bool TestIsDelimiter()
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


// Returns true until there is nothing left to preprocess
bool preprocess(TOME* const tome)
{
    //  Preprocesing assumptions (for now):
    //  1) A preprocessing directive takes up exactly one line, unless escaped (like C/C++).
    //  [ assemblyDruid::NOTE ] it would be kind of neat to have #s# #e# for "start" and "end",
    //  (or whatever syntax) eliminating the need for littering the directives with / marks.
    //  1.2) It follows that each directive will end with "\n"
    //   1. Deterimine what kind of directive we are dealing with. Right now, this is just "summon"
    //   2. Pass this directive to "apply()" so that magical stuff can happen.

    char*  file_buffer = NULL;
    char*  file_name = NULL;
    size_t total_file_size = 0;
    FILE*  file_stream = NULL;

    for (size_t i = 0; i < tome->num_tome_entries; i++ )
    {
        quick_del(file_buffer);
        quick_del(file_name);
        fclose(file_stream);
        total_file_size = 0;
        // [ assemblyDruid::TODO ] check all possible return values for fopen
#if _WIN32
#pragma warning(push)
#pragma warning( disable: 4996 )
#endif // _WIN32

        file_name = (char*) malloc(tome->tome_entries[i]->value_buffer_len * sizeof(char));
        file_name = tome->tome_entries[i]->value;
        file_stream = fopen(file_name, "r" );

#if _WIN32
#pragma warning(push)
#pragma warning( disable: 4996 )
#endif // _WIN32

        if (file_stream != NULL)
        {
            // Find the file len, allocate appropriate quantity of memory
            fseek(file_stream, 0, SEEK_END);
            total_file_size = ftell( file_stream );
            file_buffer = (char*) calloc(total_file_size, sizeof(char));
            // Reset file position, read file contents into buffer
            fseek(file_stream, 0, SEEK_SET);
            fread(file_buffer, sizeof(char), total_file_size, file_stream);
        }
        else
        {
            // [ assemblyDruid::TODO ] unified system for error handling
            dbgPrint("[ FATAL ] Could not open the file.\n");
            assert(0);
        }

        // Begin parsing the file_buffer for '#'
        for (size_t cursor_position = 0; cursor_position < total_file_size; cursor_position++)
        {
            size_t directive_length = 0;
            char* directive_source = NULL;
            char* directive_keyword = NULL;
            size_t line_position_start = 0;
            size_t line_position_end = 0;

            if (file_buffer[cursor_position] == '#')
            {
                // '#' was found. Find '\n'
                line_position_start = cursor_position;
                for (; cursor_position < total_file_size; cursor_position++)
                {
                    char newline_found = 0;
                    if(file_buffer[cursor_position] == '\n' )
                    {
                        // '\n' was found. Extract directive
                        newline_found = 1;
                        line_position_end = cursor_position;

                        // [ assemblyDruid:TODO ] turn each assertion into valid error message -> exit()
                        directive_length = line_position_end - line_position_start + 1;
                        assert(line_position_end); // dont want negative index
                        assert(directive_length > 2);
                        directive_source = (char*) calloc(directive_length, sizeof(char));
                        size_t directive_cursor = 0;
                        for (; directive_cursor < (directive_length - 1); directive_cursor++)
                        {
                            directive_source[directive_cursor] = file_buffer[line_position_start + directive_cursor];
                        }
                        directive_source[directive_length - 1] = '\0';

                        // Directive Extracted. Determine directive type. (look for first space)
                        dbgPrint("[ PREPROCESSING DIRECTIVE SOURCE ] %s\n", directive_source);

                        assert(directive_source[0] == '#');
                        size_t directive_source_cursor = 1;
                        for (; directive_source_cursor < directive_length; directive_source_cursor++)
                        {
                            if(directive_source[directive_source_cursor] == ' ')
                            {
                                if(directive_keyword) { free(directive_keyword); }
                                directive_keyword = (char*) calloc(directive_source_cursor, sizeof(char));
                                size_t n = 0;
                                for (; n < directive_source_cursor; n++)
                                {
                                    directive_keyword[n] = directive_source[n + 1];
                                }
                                directive_keyword[directive_source_cursor - 1] = '\0';
                                break;
                            }
                        }
                        dbgPrint("[ POTENTIAL DIRECTIVE TYPE ] %s\n", directive_keyword);
                        // [ assemblyDruid::NOTE::TODO ] here we get into string comparison. We can intern strings,
                        // or we can hash them. For now, for the sake of getting something working and since we can
                        // note deliberate on which would be better ( or other options in general ), i'm just going
                        // to do stuipd, expensive string comparison.
                        //
                        // This is extra extra stupid as we will have to compare between every "string" in the
                        // list of possible directive types. (i.e. "summon" versus "if", etc). For the time being,
                        // we're only checking for "summon". We will need to have this discussion before adding more
                        // possible directive types.

                        // Directive type determined. Do the thing.
                        if (!strncmp(directive_keyword, "summon", directive_source_cursor))
                        {
                            dbgPrint("[ DIRECTIVE DETECTED ] %s, %zd characters compared\n",
                                     directive_keyword, directive_source_cursor);
                            dbgPrint("\n");

                            /* void apply(FILE* fp, */
                            /*            long int offset, */
                            /*            PreprocessingDirective directive, */
                            /*            const char* parameter) */
                        }
                        break;
                    }

                    // [ assemblyDruid::TODO ] handle EOF case (no newline)
                    if ((cursor_position == (total_file_size -1)) && (!newline_found))
                    {
                        dbgPrint("[ FATAL ] No newline following directive\n");
                        assert(0);
                    }
                    // cursor_position = line_position_start;
                }
            }
            if (directive_source) { free(directive_source); }
            if (directive_keyword) { free(directive_keyword); }
        }

        // [ assemblyDruid::TODO ] look into setting the msvc flag for c99, if you haven't already and if possible
        fclose(file_stream);
        free(file_buffer);
    }

    quick_del(file_buffer);
    quick_del(file_name);
    fclose(file_stream);

    return true;
}

void
addTomeEntry(TOME* const tome, char_char_dict* const entry)
{
    if (tome->tome_entries_buffer_len)
    {
        // ensure there's enough room to add another entry
        if (tome->tome_entries_buffer_len > tome->num_tome_entries)
        {
            tome->tome_entries[tome->num_tome_entries] = (char_char_dict*) entry;
            tome->num_tome_entries++;
        }
        else
        {
            tome->tome_entries_buffer_len *= 2;

            // [ assemblyDruid ] .... is this right?
            if (realloc(tome->tome_entries, (tome->tome_entries_buffer_len * sizeof(char_char_dict*))))
            {
                tome->tome_entries[tome->num_tome_entries] = (char_char_dict*) entry;
                tome->num_tome_entries++;
            }
            else
            {
                dbgPrint("[ FATAL ] Unable to resize tome_entry block.\n");
                assert(0);
            }
        }
    }
    else
    {
        // allocate initial buffer len (8)
        tome->tome_entries_buffer_len = 8;
        tome->tome_entries =
            (char_char_dict**) malloc(tome->tome_entries_buffer_len * sizeof(char_char_dict*));
    }
}

void
processTome(TOME* const tome)
{
    // [ assemblyDruid::ASSUMPTION ]
    // The tome will be in the same directory as the executable,
    // with the filename "tome->druid"

    quick_del(tome->temp.file_buffer);
    tome->temp.total_file_size = 0;
    dbgPrint("\tTome cleaned.\n");

#if _WIN32
#pragma warning(push)
#pragma warning( disable: 4996 )
#endif // _WIN32

    tome->temp.file_stream = fopen("tome", "r" );
    dbgPrint("\tOpened file stream 'tome'.\n");

#if _WIN32
#pragma warning(push)
#pragma warning( disable: 4996 )
#endif // _WIN32

    if (tome->temp.file_stream != NULL)
    {
        fseek(tome->temp.file_stream, 0, SEEK_END);
        tome->temp.total_file_size = ftell( tome->temp.file_stream );
        tome->temp.file_buffer = (char*) calloc(tome->temp.total_file_size, sizeof(char));
        fseek(tome->temp.file_stream, 0, SEEK_SET);
        fread(tome->temp.file_buffer,
              sizeof(char),
              tome->temp.total_file_size,
              tome->temp.file_stream);
        dbgPrint("\tFile size determiend.\n");
    }
    else
    {
        // [ assemblyDruid::TODO ] unified system for error handling
        dbgPrint("[ FATAL ] Unable to locate a tome file for this project.\n");
        assert(0);
    }

    dbgPrint("\tTome file contents:\n%s\n", tome->temp.file_buffer);

    size_t line_start = 0;
    size_t line_number = 0;
    size_t  zero_corrected_prev_comment_char_position = 0;

    for (size_t file_index = 0; file_index < tome->temp.total_file_size; file_index++)
    {
        char _char = tome->temp.file_buffer[file_index];
        bool discard_line = false;

        // check for comments
        size_t zero_corrected_file_index = file_index + 1;
        if (_char == '/')
        {
            if (!discard_line)
            {
                if (zero_corrected_prev_comment_char_position)
                {
                    if (zero_corrected_file_index - zero_corrected_prev_comment_char_position == 1)
                    {
                        discard_line = true;
                        /* dbgPrint("\tComment found on line %zd. discard_line set to [ true ]\n", line_number); */
                    }

                    zero_corrected_prev_comment_char_position = 0;
                }
                else
                {
                    zero_corrected_prev_comment_char_position = zero_corrected_file_index;
                    /* dbgPrint("\tFound an initial comment char at position: %zd\n", file_index); */
                }
            }
        }

        // check for new line
        if (_char == '\n')
        {
            char_char_dict* temp_entry = NULL;

            if (!discard_line)
            {
                bool   at_symbol_found_on_this_line = false;
                size_t at_symbol_index = 0;
                size_t num_non_space_characters_preceeding_at_sign = 0;

                for (size_t sub_index = line_start;
                     sub_index < file_index;
                     sub_index++)
                {
                    if (tome->temp.file_buffer[sub_index] == '@')
                    {
                        dbgPrint("\t'@' found on line %zd.\n", line_number);
                        if (at_symbol_found_on_this_line)
                        {
                            dbgPrint("[ FATAL ] Only one spell is allowed per tome line.\n");
                            assert(0);
                        }
                        else
                        {
                            at_symbol_index = sub_index;

                            dbgPrint("\tAllocating memory for new char_char_dict.\n");

                            temp_entry = (char_char_dict*) malloc(sizeof(char_char_dict));
                            temp_entry->key_buffer_len = (sub_index - line_start);
                            temp_entry->key =
                                (char*) calloc(num_non_space_characters_preceeding_at_sign, sizeof(char));

                            size_t temp_entry_index = 0;
                            for (size_t super_sub_index = line_start;
                                 super_sub_index < at_symbol_index;
                                 super_sub_index++)
                            {
                                if (tome->temp.file_buffer[super_sub_index] != ' ')
                                {
                                    temp_entry->key[temp_entry_index] = tome->temp.file_buffer[super_sub_index];
                                    temp_entry_index++;
                                }
                            }

                            //
                            //
                            // [ assemblyDruid::REMEMBER ]
                            // Pickig up '@' in lines that should be ignored.
                            //
                            //

                            dbgPrint("\tAdded key { %s } to tome.\n", temp_entry->key);
                        }
                    }

                    if ((!at_symbol_found_on_this_line) &&
                        (tome->temp.file_buffer[sub_index] != ' '))
                    {
                        num_non_space_characters_preceeding_at_sign++;
                    }

                    if (at_symbol_found_on_this_line)
                    {
                        char* non_space_value_character_buffer =
                            (char*) malloc((file_index - at_symbol_index) * sizeof(char));
                        size_t non_space_value_characters = 0;

                        for (size_t super_sub_index = at_symbol_index;
                             super_sub_index < file_index;
                             super_sub_index++)
                        {
                            if (tome->temp.file_buffer[super_sub_index] != ' ')
                            {

                                non_space_value_character_buffer[non_space_value_characters] =
                                    tome->temp.file_buffer[super_sub_index];
                                non_space_value_characters++;
                            }
                        }

                        temp_entry->value_buffer_len = non_space_value_characters;
                        temp_entry->value = (char*) malloc(non_space_value_characters * sizeof(char));
                        for(size_t temp_value_index = 0;
                            temp_value_index < non_space_value_characters;
                            temp_value_index++)
                        {
                            temp_entry->value[temp_value_index] =
                                non_space_value_character_buffer[temp_value_index];
                        }

                        quick_del(non_space_value_character_buffer);

                        addTomeEntry(tome, temp_entry);
                        dbgPrint("\tAdded tome entry: {key: %s, value: %s}\n",
                                 temp_entry->key,
                                 temp_entry->value);
                    }
                }
            }
            line_start = file_index + 1;
            line_number++;
        }
    }

    quick_del(tome->temp.file_buffer);
    fclose(tome->temp.file_stream);
    tome->temp.total_file_size = 0;
}

int
main(int argc, char** argv)
{
    for (int i = 0; i < argc; i++)
    {
        dbgPrint("argv[%d]: %s\n", i, argv[i]);
    }

    //
    // preprocessor
    //
    TOME tome;
    tome.tome_entries_buffer_len = 0;
    tome.num_tome_entries = 0;
    tome.temp.file_buffer = NULL;
    tome.temp.total_file_size = 0;
    tome.temp.file_stream = NULL;

    dbgPrint("Processing tome...\n");
    processTome(&tome);

    // [ assemblyDruid::DEBUG ]
    dbgPrint("tome has %zd entries at %zd bytes\n", tome.num_tome_entries, tome.tome_entries_buffer_len);
    /* for (size_t tome_index = 0; tome_index < tome.num_tome_entries; tome_index++) */
    /* { */
    /*     dbgPrint("tome entry %zd of %zd: {key: %s, value: %s}\n", */
    /*            tome_index, */
    /*            tome.num_tome_entries, */
    /*            tome.tome_entries[tome_index].key, */
    /*            tome.tome_entries[tome_index].value); */
    /* } */

    dbgPrint("Preprocessing tome entries...\n");
    preprocess(&tome);

    //
    // lexer
    //
    assert(TestIsAlphabetical());
    assert(TestIsNumeric());
    assert(TestIsDelimiter());

    dbgPrint("\n[ SUCCESS ]\n");
}
