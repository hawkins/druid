#ifndef __dString__
#define __dString__ 1

#include <type_tools.h>
#include <macro_tools.h>
#include <debug_tools.h>

#include <stdlib.h>

typedef struct
{
    char* volatile data;
    const size_t length;
    const size_t bytes;
} String;

__internal__ __inline__ String*
StringInit(const char* str)
{
    if (str)
    {
        String* uStr = (String*) calloc(1, sizeof(String));
        size_t* non_const_length = (size_t*) &(uStr->length);
        *non_const_length = strlen(str) + 1; // strlen does not count '\0'

        uStr->data = (char*) malloc(sizeof(char) * *non_const_length);
        memcpy(uStr->data, str, *non_const_length);

        return uStr;
    }

    return NULL;
}

__internal__ __inline__ bool
StringDestroy(String* const uStr)
{
    if(uStr && uStr->data)
    {
        free(uStr->data);
        free(uStr);
        return true;
    }

    return false;
}

// [ cfarvin::TODO ]
/* __internal__ __inline__ uDynamicArray* */
/* StringSubstringIndices(StringArray* const uSA, const char* sub_string) */
/* {} */


#endif // __dString__
