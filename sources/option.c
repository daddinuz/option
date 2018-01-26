/*
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 *
 * Copyright (c) 2018 Davide Di Carlo
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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include "option.h"

/*
 * Common
 */
static void
panic(const char *file, size_t line, const char *format, va_list args, const char *details)
__attribute__((__nonnull__(1, 3), __format__(__printf__, 3, 0), __noreturn__));

/*
 * Option
 */
const Option None = {.__data=NULL};

Option Option_new(void *data) {
    Option self = (data) ? (Option) {.__data=data} : None;
    return self;
}

bool Option_isSome(Option self) {
    return NULL != self.__data;
}

bool Option_isNone(Option self) {
    return NULL == self.__data;
}

void *__Option_expect(const char *__file, size_t __line, Option self, const char *format, ...) {
    assert(__file);
    assert(format);
    if (Option_isNone(self)) {
        va_list args;
        va_start(args, format);
        panic(__file, __line, format, args, NULL);
    }
    return self.__data;
}

void *__Option_unwrap(const char *__file, size_t __line, Option self) {
    assert(__file);
    return __Option_expect(__file, __line, self, "%s", "Unable to unwrap value.");
}

Option Option_map(Option self, Option mapFn(Option)) {
    assert(mapFn);
    return mapFn(self);
}

Option Option_mapOr(Option self, Option def, Option mapFn(Option)) {
    assert(mapFn);
    return Option_isSome(self) ? mapFn(self) : def;
}

Option Option_mapOrElse(Option self, Option defFn(void), Option mapFn(Option)) {
    assert(defFn);
    assert(mapFn);
    return Option_isSome(self) ? mapFn(self) : defFn();
}

/*
 * Error
 */
const Error Ok = Error_new("Ok");

void __Error_expect(const char *__file, size_t __line, Error self, const char *format, ...) {
    assert(__file);
    assert(format);
    if (Ok != self) {
        va_list args;
        va_start(args, format);
        panic(__file, __line, format, args, NULL);
    }
}

void __Error_unwrap(const char *__file, size_t __line, Error self) {
    assert(__file);
    __Error_expect(__file, __line, self, "%s", self->message);
}

/*
 * Result
 */
Result Result_ok(void *data) {
    return (Result) {.__data=data, .__error=Ok, .__details=NULL};
}

Result Result_error(Error error) {
    assert(error);
    assert(Ok != error);
    return (Result) {.__data=NULL, .__error=error, .__details=NULL};
}

Result Result_errorWithDetails(Error error, const char *details) {
    assert(error);
    assert(details);
    assert(Ok != error);
    return (Result) {.__data=NULL, .__error=error, .__details=details};
}

bool Result_isOk(Result self) {
    return Ok == self.__error;
}

bool Result_isError(Result self) {
    return Ok != self.__error;
}

void *__Result_expect(const char *__file, size_t __line, Result self, const char *format, ...) {
    assert(__file);
    assert(format);
    if (Result_isError(self)) {
        va_list args;
        va_start(args, format);
        panic(__file, __line, format, args, self.__details);
    }
    return self.__data;
}

void *__Result_unwrap(const char *__file, size_t __line, Result self) {
    assert(__file);
    return __Result_expect(__file, __line, self, "%s", self.__error->message);
}

Error Result_inspect(Result self) {
    return self.__error;
}

const char *Result_explain(Result self) {
    return self.__error->message;
}

const char *Result_details(Result self) {
    return self.__details ? self.__details : "";
}

/*
 * Common
 */
void panic(const char *file, size_t line, const char *format, va_list args, const char *details) {
    assert(file);
    assert(format);
    fflush(stdout);
    fprintf(stderr, "\nAt: %s:%zu\nError: ", file, line);
    vfprintf(stderr, format, args);
    if (details) {
        fprintf(stderr, "\nDetails: %s", details);
    }
    fputs("\n", stderr);
    va_end(args);
    exit(1);
}
