#ifndef __MACRO_TOOLS_H__
#define __MACRO_TOOLS_H__



//
// Decorators
//
#ifndef __call__
#define __call__ /* NOTICE: CALL */
#endif // __call__

#ifndef _mut_
#define _mut_ /* NOTICE: MUTABLE */
#endif // _mut_

#ifndef __internal__
#define __internal__ static
#endif // __internal__

#ifndef __external__
#define __external__ extern // functions
#endif // __external__



//
// Passifiers
//
#ifdef _WIN32
#define restrict __restrict
#endif // WIN32

#endif // __MACRO_TOOLS_H__
