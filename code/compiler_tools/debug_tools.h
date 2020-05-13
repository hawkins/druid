#ifndef __DEBUG_TOOLS_H__
#define __DEBUG_TOOLS_H__


//
// Set to 0 to disable all debugging functions
//
#define __DEBUG__ 1
//
//
//

#if  __DEBUG__
#define __inline__    /* INLINE REMOVED */
#else // __DEBUG__
#define __inline__    inline
#endif // __DEBUG__


#ifndef functionFired
#if __DEBUG__
#define functionFired  printf("[ debug ] function fired: %s\n", __func__);
#else
#define functionFired  /* Debugging is disabled: functionFired */
#endif // __DEBUG__
#endif // functionFired


/*
 * Error Reporting
 *
 * Error reporting:
 *    1. `uError_v` (Verbose): Print a formatted error with file
 *        and line information to stderr (does not halt).
 *    2. `uError`: Print a formatted error without file and line
 *        information to stderr (does not halt).
 *
 * Debug printing:
 *    1. `uDebugPrint_v` (Verbose): Print a formatted debug string
 *        with file and line information to stderr. (does not
 *        halt).
 *    2. `uDebugPrint`: Print a formatted debug string without
 *        file and line information to stderr. (does not halt).
 *
 * Debug assertions:
 *    1. `uAssert_v` (Verbose): If the condition is false,
 *        print a message with file and line information to
 *        stderr and exit the process with code 666.
 *    1. `uAssert` (Verbose): If the condition is false,
 *        print a message without file and line information to
 *        stderr and exit the process with code 666. *
 * The length of the message/error is determined by the
 * `MAX_ERROR_LEN` macro set in the lines below.
 *
 */
#if __DEBUG__
#define MAX_ERROR_LEN 256
char _error_buffer[MAX_ERROR_LEN];
char _message_buffer[MAX_ERROR_LEN];

// Error_v()
#define Error_v(...)                                            \
    snprintf(_message_buffer, MAX_ERROR_LEN, __VA_ARGS__);      \
    snprintf(_error_buffer,                                     \
             MAX_ERROR_LEN,                                     \
             "[ error ] [ %s::%d ] %s\n",                       \
             __FILE__,                                          \
             __LINE__,                                          \
             _message_buffer);                                  \
    fputs(_error_buffer, stderr);                               \
    fflush(stderr)

// Error()
#define Error(...)                                              \
    snprintf(_message_buffer, MAX_ERROR_LEN, __VA_ARGS__);      \
    snprintf(_error_buffer,                                     \
             MAX_ERROR_LEN,                                     \
             "[ error ] %s",                                    \
             _message_buffer);                                  \
    fputs(_error_buffer, stderr)


// DebugPrint_v()
#define DebugPrint_v(...)                                       \
    snprintf(_message_buffer, MAX_ERROR_LEN, __VA_ARGS__);      \
    snprintf(_error_buffer,                                     \
             MAX_ERROR_LEN,                                     \
             "[ debug ] [ %s::%d ] %s",                         \
             __FILE__,                                          \
             __LINE__,                                          \
             _message_buffer);                                  \
    fputs(_error_buffer, stderr);                               \
    fflush(stderr)

// DebugPrint()
#define DebugPrint(...)                                         \
    snprintf(_message_buffer, MAX_ERROR_LEN, __VA_ARGS__);      \
    snprintf(_error_buffer,                                     \
             MAX_ERROR_LEN,                                     \
             "[ debug ] %s",                                    \
             _message_buffer);                                  \
    fputs(_error_buffer, stderr);                               \
    fflush(stderr)

// AssertMsg()
#define AssertMsg(cond, ...)                                    \
    if (!cond) {                                                \
        snprintf(_message_buffer, MAX_ERROR_LEN, __VA_ARGS__);  \
        snprintf(_error_buffer,                                 \
                 MAX_ERROR_LEN,                                 \
                 "[ assertion ] %s",                            \
                 _message_buffer);                              \
        fputs(_error_buffer, stderr);                           \
        fflush(stderr);                                         \
        exit(666); }

// Assert_v()
#define AssertMsg_v(cond, ...)                                  \
    if (!(cond)) {                                              \
        snprintf(_message_buffer, MAX_ERROR_LEN, __VA_ARGS__);  \
        snprintf(_error_buffer,                                 \
                 MAX_ERROR_LEN,                                 \
                 "[ assertion ] [ %s::%d ] %s",                 \
                 __FILE__,                                      \
                 __LINE__,                                      \
                 _message_buffer);                              \
        fputs(_error_buffer, stderr);                           \
        fflush(stderr);                                         \
        exit(666); }

// Assert()
#define Assert(cond)                            \
    if (!((long long) (cond))) { exit(666); }   \


#else // __DEBUG__
#define Error_v(...)        /* System debugging is disabled */
#define Error(...)          /* System debugging is disabled */
#define DebugPrint_v(...)   /* System debugging is disabled */
#define DebugPrint(...)     /* System debugging is disabled */
#define Assert(cond, ...)   /* System debugging is disabled */
#define Assert_v(cond, ...) /* System debugging is disabled */
#endif // __DEBUG__

#endif // __DEBUG_TOOLS_H__
