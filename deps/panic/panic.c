/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Davide Di Carlo
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <threads.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>
#if PANIC_UNWIND_SUPPORT
#define UNW_LOCAL_ONLY
#include <libunwind.h>
#endif
#include "panic.h"

#ifdef __WIN32
#define NEWLINE "\r\n"
#else
#define NEWLINE "\n"
#endif

static PanicHandler globalPanicHandler = NULL;

static void terminate(const char *restrict trace, const char *restrict format, va_list args)
__attribute__((__noreturn__, __nonnull__(1, 2), __format__(__printf__, 2, 0)));

static void stacktrace(FILE *stream)
__attribute__((__nonnull__));

PanicHandler panic_registerHandler(const PanicHandler handler) {
    const PanicHandler backup = globalPanicHandler;
    globalPanicHandler = handler;
    return backup;
}

void panic_abort(const char *const trace, const char *const format, ...) {
    assert(NULL != trace);
    assert(NULL != format);
    va_list args;
    va_start(args, format);
    terminate(trace, format, args);
}

void panic_vabort(const char *const trace, const char *const format, va_list args) {
    assert(NULL != trace);
    assert(NULL != format);
    terminate(trace, format, args);
}

void terminate(const char *const trace, const char *const format, va_list args) {
    assert(NULL != trace);
    assert(NULL != format);
    fputs(NEWLINE, stderr);
    stacktrace(stderr);
    fprintf(stderr, "   At: '%s'" NEWLINE, trace);

    if (0 != errno) {
        fprintf(stderr, "Error: (%d) %s" NEWLINE, errno, strerror(errno));
    }

    fputs("Cause: ", stderr);
    vfprintf(stderr, format, args);
    fputs(NEWLINE, stderr);
    va_end(args);

    if (NULL != globalPanicHandler) {
        globalPanicHandler();
    }

    thrd_exit(EXIT_FAILURE);
}

void stacktrace(FILE *const stream) {
    assert(NULL != stream);
    (void) stream;
#if PANIC_UNWIND_SUPPORT

#define UNWIND_STEPS        8u
#define UNWIND_FN_NAME_LEN  32u

    char buffer[UNWIND_STEPS][UNWIND_FN_NAME_LEN + 1] = {{ 0 }};
    unw_context_t context = { 0 };
    unw_cursor_t cursor = { 0 };
    const int previousError = errno;
    unsigned step = 0;

    unw_getcontext(&context);
    unw_init_local(&cursor, &context);

    // skip: terminate and panics function calls
    for (unsigned i = 0; i < 2; ++i) {
        if (unw_step(&cursor) <= 0) {
            errno = previousError;  // restore errno
            return;                 // something wrong, exit
        }
    }

    while (step < UNWIND_STEPS     &&
           unw_step(&cursor) > 0   &&
           unw_get_proc_name(&cursor, buffer[step], UNWIND_FN_NAME_LEN, NULL) == 0 &&
           strcmp("main", buffer[step++]) != 0) {}

    if (0 < step) {
        fputs("Traceback (most recent call last):" NEWLINE, stream);

        if (0 != strcmp("main", buffer[step - 1])) {
            fputs("  [ ]: (...)" NEWLINE, stream);
        }

        for (unsigned i = 1; i < step; ++i) {
            fprintf(stream, "  [%d]: (%s)" NEWLINE, i - 1, buffer[step - i]);
        }

        fprintf(stream, "  ->-: (%s) current function" NEWLINE, buffer[0]);
        fputs(NEWLINE, stream);
    }

    errno = previousError;  // restore errno
#endif
}
