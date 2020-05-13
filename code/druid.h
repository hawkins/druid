#ifndef __DRUID_H__
#define __DRUID_H__


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include <debug_tools.h>
#include <macro_tools.h>
#include <type_tools.h>
#include <preprocessor_tools.h>
#include <character_tools.h>
#include <lexer_tools.h>
#include <operator_tools.h>



#ifndef DRUID_DEBUG
#define DRUID_DEBUG 1
#endif // DRUID_DEBUG

#ifndef mlstring // multi_line_string
#define mlstring(x) (#x)
#endif // mlstring

#ifndef TokenStreamSize
#define TokenStreamSize (sizeof(TokenStream)/sizeof(TokenStream[0]))
#endif // TokenStreamSize


#endif // __DRUID_H__
