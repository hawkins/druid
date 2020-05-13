/*
  DynamicArray Limitations
  -------------------------
  - DynamicArray does not account for functions which modify the `data`
  member directly, as it cannot tell how many elements may have been
  added. To do this safely, the dynamic array must have been set up
  manually or with a call to DAInit() which allocated to proper number
  of bytes before the direct modification. If this is the case, ask
  yourself: "Do I really need a dynamic array?". If the answer is still
  yes, just remember that DynamicArray only tracks element-counts and
  resizes properly via functions designed to interact with and modify
  DynamicArrays, like DAPush(). You may run into issues with assertions
  and sanity checks within the DynamicArray functions if you attempt to
  use them after altering the `data` member yourself.
*/

#ifndef __DynamicArray__
#define __DynamicArray__ 1

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>



// Optimize for random access, "push back", "pop last",
// capping out at 2^16 elements via IEEE size_t.
typedef struct
{
    _mut_ void* volatile data;

    const          size_t datatype_size;
    volatile const size_t length;
    volatile const size_t scaling_factor;
    volatile const size_t max_length;
} DynamicArray;


// [ cfarvin::TODO ] Remove after use in Druid
#if __GNUC__
#pragma GCC diagnostic ignored "-Wunused-function"
#endif // __GNUC__
#define DAInit(type) API_DAInit(sizeof(type))
__internal__ __inline__ DynamicArray*
API_DAInit(const size_t datatypesize_in)
{
    AssertMsg_v(datatypesize_in, "Data type size must be non-zero.\n");
    DynamicArray* const da = (DynamicArray*) calloc(1, sizeof(DynamicArray));

    // Initialize statics
    size_t* non_const_length = (size_t*) &(da->length);
    size_t* non_const_scaling_factor = (size_t*) &(da->scaling_factor);
    size_t* non_const_max_length = (size_t*) &(da->max_length);
    size_t* non_const_datatype_size = (size_t*) &(da->datatype_size);

    *non_const_length = 0;
    *non_const_scaling_factor = 2;
    *non_const_max_length = 2;
    *non_const_datatype_size = datatypesize_in;

    // Initialize Dynamics
    da->data = (void*) malloc(*non_const_datatype_size * da->max_length);

    return da;
}
// [ cfarvin::TODO ] Remove after use in Druid
#if __GNUC__
#pragma GCC diagnostic pop
#endif // __GNUC__


// [ cfarvin::TODO ] Remove after use in Druid
#if __GNUC__
#pragma GCC diagnostic ignored "-Wunused-function"
#endif // __GNUC__
#define DAPush(da, data_in) API_DAPush(da, VPPC_STR_LITERAL(void** const) data_in)
__internal__ __inline__ bool
API_DAPush(_mut_ DynamicArray* const restrict da,
           const void**        const restrict data_in)
{
    AssertMsg_v(da,      "Null DynamicArray ptr provided.\n");
    AssertMsg_v(data_in, "Null data ptr pvoided.\n");

    if (da && data_in)
    {
        // [ cfarvin::TODO ] Properly check for failure on realloc
        if (da->length >= da->max_length)
        {
            size_t* non_const_max_length = (size_t*) &(da->max_length);
            *non_const_max_length = da->max_length * da->scaling_factor;
            void* allocated = realloc(da->data, (da->datatype_size * da->max_length));
            if (allocated)
            {
                da->data = allocated;
            }
        }

        memcpy((char*)da->data + (da->length * da->datatype_size),
               data_in,
               da->datatype_size);

        size_t* non_const_length = (size_t*) &(da->length);
        *non_const_length = *non_const_length + 1;

        return true;
    }

    return false;
}
// [ cfarvin::TODO ] Remove after use in Druid
#if __GNUC__
#pragma GCC diagnostic pop
#endif // __GNUC__


// [ cfarvin::TODO ] Remove after use in Druid
#if __GNUC__
#pragma GCC diagnostic ignored "-Wunused-function"
#endif // __GNUC__
__internal__ __inline__ void*
DAIndex(_mut_ DynamicArray* const restrict da,
        const size_t index)
{
    AssertMsg_v(da, "Null DynamicArray ptr pvoided.\n");
    AssertMsg_v(index < da->length,
                "Index [ %zd ] surpasses dynamic array length: [ %zd ].\n",
                index,
                da->length);

    if (da && (index < da->length))
    {
        return (void*)((char*)da->data + (index * da->datatype_size));
    }

    return NULL;
}
// [ cfarvin::TODO ] Remove after use in Druid
#if __GNUC__
#pragma GCC diagnostic pop
#endif // __GNUC__


// [ cfarvin::TODO ] Remove after use in Druid
#if __GNUC__
#pragma GCC diagnostic ignored "-Wunused-function"
#endif // __GNUC__
__internal__ __inline__ bool
DAPop(_mut_ DynamicArray* const restrict da)
{
    AssertMsg_v(da,         "Null DynamicArray ptr pvoided.\n");
    AssertMsg_v(da->length, "Dynamic array length must be non-zero\n");
    AssertMsg_v(da->length <= da->max_length,
                "Dynamic array length must be less than the maximum length");

    if (da && da->length && da->length <= da->max_length)
    {
        size_t* non_const_length = (size_t*) &(da->length);
        *non_const_length = (*non_const_length) - 1;

        return true;
    }

    return false;
}
// [ cfarvin::TODO ] Remove after use in Druid
#if __GNUC__
#pragma GCC diagnostic pop
#endif // __GNUC__


// [ cfarvin::TODO ] Remove after use in Druid
#if __GNUC__
#pragma GCC diagnostic ignored "-Wunused-function"
#endif // __GNUC__
__internal__ __inline__ bool
DAFitToSize(_mut_ DynamicArray* const restrict da)
{
    AssertMsg_v(da,      "Null DynamicArray ptr provided.\n");

    if (da && (da->length > da->scaling_factor))
    {
        size_t* non_const_max_length = (size_t*) &(da->max_length);
        *non_const_max_length = da->length;
        da->data = realloc(da->data, (da->datatype_size * da->max_length));

        return true;
    }

    return false;
}
// [ cfarvin::TODO ] Remove after use in Druid
#if __GNUC__
#pragma GCC diagnostic pop
#endif // __GNUC__


// [ cfarvin::TODO ] Remove after use in Druid
#if __GNUC__
#pragma GCC diagnostic ignored "-Wunused-function"
#endif // __GNUC__
__internal__ __inline__ bool
DASetScalingFactor(_mut_ DynamicArray* const restrict da,
                   const size_t scaling_factor_in)
{
    AssertMsg_v(da, "Null DynamicArray ptr provided.\n");
    AssertMsg_v(scaling_factor_in > 1,
                "Scaling factor must be greater than 1.\n");

    if (da && scaling_factor_in > 1)
    {
        size_t* non_const_scaling_factor = (size_t*) &da->scaling_factor;
        *non_const_scaling_factor = scaling_factor_in;

        return true;
    }

    return false;
}
// [ cfarvin::TODO ] Remove after use in Druid
#if __GNUC__
#pragma GCC diagnostic pop
#endif // __GNUC__


// [ cfarvin::TODO ] Remove after use in Druid
#if __GNUC__
#pragma GCC diagnostic ignored "-Wunused-function"
#endif // __GNUC__
__internal__ __inline__ bool
DADestroy(_mut_ DynamicArray* const restrict da)
{
    if (da && da->data)
    {
        free(da->data);
        free(da);

        return true;
    }

    return false;
}
// [ cfarvin::TODO ] Remove after use in Druid
#if __GNUC__
#pragma GCC diagnostic pop
#endif // __GNUC__



#endif // __DynamicArray
