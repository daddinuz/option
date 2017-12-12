/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   December 12, 2017 
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include "option.h"

/*
 * Common
 */
static void
Option_panic(const char *file, size_t line, const char *format, va_list args)
__attribute__((__nonnull__, __noreturn__, __format__(__printf__, 3, 0)));

/*
 * Mutable Option
 */
const MutableOption MutableOption_None = {.__some=NULL};

MutableOption
MutableOption_new(void *value) {
    MutableOption self = (value) ? (MutableOption) {.__some=value} : MutableOption_None;
    return self;
}

bool
MutableOption_isSome(MutableOption self) {
    return NULL != self.__some;
}

bool
MutableOption_isNone(MutableOption self) {
    return NULL == self.__some;
}

void *
__MutableOption_expect(const char *__file, size_t __line, MutableOption self, const char *format, ...) {
    assert(__file);
    assert(format);
    if (MutableOption_isNone(self)) {
        va_list args;
        va_start(args, format);
        Option_panic(__file, __line, format, args);
    }
    return self.__some;
}

void *
__MutableOption_unwrap(const char *__file, size_t __line, MutableOption self) {
    assert(__file);
    return __MutableOption_expect(__file, __line, self, "%s", "Unable to unwrap value.");
}

MutableOption
MutableOption_map(MutableOption self, MutableOption mapFn(MutableOption)) {
    assert(mapFn);
    return mapFn(self);
}

MutableOption
MutableOption_mapOr(MutableOption self, MutableOption def, MutableOption mapFn(MutableOption)) {
    assert(mapFn);
    return MutableOption_isSome(self) ? mapFn(self) : def;
}

MutableOption
MutableOption_mapOrElse(MutableOption self, MutableOption defFn(void), MutableOption mapFn(MutableOption)) {
    assert(defFn);
    assert(mapFn);
    return MutableOption_isSome(self) ? mapFn(self) : defFn();
}

/*
 * Immutable Option
 */
const ImmutableOption ImmutableOption_None = {.__some=NULL};

ImmutableOption
ImmutableOption_new(const void *value) {
    ImmutableOption self = (value) ? (ImmutableOption) {.__some=value} : ImmutableOption_None;
    return self;
}

bool
ImmutableOption_isSome(ImmutableOption self) {
    return NULL != self.__some;
}

bool
ImmutableOption_isNone(ImmutableOption self) {
    return NULL == self.__some;
}

const void *
__ImmutableOption_expect(const char *__file, size_t __line, ImmutableOption self, const char *format, ...) {
    assert(__file);
    assert(format);
    if (ImmutableOption_isNone(self)) {
        va_list args;
        va_start(args, format);
        Option_panic(__file, __line, format, args);
    }
    return self.__some;
}

const void *
__ImmutableOption_unwrap(const char *__file, size_t __line, ImmutableOption self) {
    assert(__file);
    return __ImmutableOption_expect(__file, __line, self, "%s", "Unable to unwrap value.");
}

ImmutableOption
ImmutableOption_map(ImmutableOption self, ImmutableOption mapFn(ImmutableOption)) {
    assert(mapFn);
    return mapFn(self);
}

ImmutableOption
ImmutableOption_mapOr(ImmutableOption self, ImmutableOption def, ImmutableOption mapFn(ImmutableOption)) {
    assert(mapFn);
    return ImmutableOption_isSome(self) ? mapFn(self) : def;
}

ImmutableOption
ImmutableOption_mapOrElse(ImmutableOption self, ImmutableOption defFn(void), ImmutableOption mapFn(ImmutableOption)) {
    assert(defFn);
    assert(mapFn);
    return ImmutableOption_isSome(self) ? mapFn(self) : defFn();
}

/*
 *
 */
void Option_panic(const char *const file, const size_t line, const char *const format, va_list args) {
    assert(file);
    assert(format);
    fflush(stdout);
    fprintf(stderr, "\nAt: %s:%zu\nError: ", file, line);
    vfprintf(stderr, format, args);
    fputs("\n", stderr);
    va_end(args);
    exit(1);
}
