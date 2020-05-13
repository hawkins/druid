#ifndef __TYPE_TOOLS__
#define __TYPE_TOOLS__

#include <stdint.h>
#include <stddef.h>

#include <macro_tools.h>



// vppc = void pointer pointer const
#define VPPC_STR_LITERAL(vppc) (vppc)



//
// Fundamental Types
//
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;
typedef float    r32;
typedef double   r64;

typedef enum
{
    false = 0,
    true = 1
} bool;



//
// Endian Types
//
union
{
    s16 dword;
    s8  word[2];
} EndiannessUnion;


typedef enum
{
    LITTLE_ENDIAN = 0,
    BIG_ENDIAN    = 1
} Endianness;
Endianness SystemEndianness;


__internal__ __call__ void
DetermineSystemEndianness()
{
    EndiannessUnion.dword = 1;
    if (EndiannessUnion.word[0])
    {
        SystemEndianness = LITTLE_ENDIAN;
        return;
    }

    SystemEndianness = BIG_ENDIAN;
}


#endif // __TYPE_TOOLS__
