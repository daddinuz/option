/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   January 10, 2018
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
panic(const char *file, size_t line, const char *format, va_list args)
__attribute__((__nonnull__, __noreturn__, __format__(__printf__, 3, 0)));

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
        panic(__file, __line, format, args);
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
const struct __Error *const Ok = Error_new("Ok");

void __Error_expect(const char *__file, size_t __line, Error self, const char *format, ...) {
    assert(__file);
    assert(format);
    if (Ok != self) {
        va_list args;
        va_start(args, format);
        panic(__file, __line, format, args);
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
    return (Result) {.__data=data, .__error=Ok};
}

Result Result_error(Error error) {
    assert(error);
    assert(Ok != error);
    return (Result) {.__data=NULL, .__error=error};
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
        panic(__file, __line, format, args);
    }
    return self.__data;
}

void *__Result_unwrap(const char *__file, size_t __line, Result self) {
    assert(__file);
    return __Result_expect(__file, __line, self, "%s", self.__error->message);
}

const struct __Error *Result_inspect(Result self) {
    return self.__error;
}

const char *Result_explain(Result self) {
    return self.__error->message;
}

/*
 * Common
 */
void panic(const char *const file, const size_t line, const char *const format, va_list args) {
    assert(file);
    assert(format);
    fflush(stdout);
    fprintf(stderr, "\nFrom: %s:%d\nAt: %s:%zu\nError: ", __FILE__, __LINE__, file, line);
    vfprintf(stderr, format, args);
    fputs("\n", stderr);
    va_end(args);
    exit(1);
}
