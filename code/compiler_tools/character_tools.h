#ifndef __CHARACTER_TOOLS__
#define __CHARACTER_TOOLS__


__internal__ __inline__  bool
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


__internal__ __inline__ bool
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

// [ cfarvin::TODO ] Remove after use in Druid
#if __GNUC__
#pragma GCC diagnostic ignored "-Wunused-function"
#endif // __GNUC__
__internal__ __inline__ bool
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
// [ cfarvin::TODO ] Remove after use in Druid
#if __GNUC__
#pragma GCC diagnostic pop
#endif // __GNUC__


__internal__ __inline__ bool
IsWhitespace(const char character)
{
    if (character == ' ')
    {
	return true;
    }

    return false;
}



#endif // __CHARACTER_TOOLS__
