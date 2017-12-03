/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   December 03, 2017 
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "immutable_option.h"

static void
__ImmutableOption_panic(const char *const file, const size_t line, const char *const message) {
    fflush(stdout);
    fprintf(stderr, "\nAt: %s:%zu\nError: %s\n", file, line, message);
    exit(1);
}

const ImmutableOption ImmutableOption_None = {.__some=NULL};

ImmutableOption
ImmutableOption_new(void *value) {
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
__ImmutableOption_expect(const char *__file, size_t __line, ImmutableOption self, const char *message) {
    assert(message);
    assert(__file);
    if (ImmutableOption_isNone(self)) {
        __ImmutableOption_panic(__file, __line, message);
    }
    return self.__some;
}

const void *
__ImmutableOption_unwrap(const char *__file, size_t __line, ImmutableOption self) {
    assert(__file);
    return __ImmutableOption_expect(__file, __line, self, "Unable to unwrap value.");
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
