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
#include "mutable_option.h"

static void
__MutableOption_panic(const char *const file, const size_t line, const char *const message) {
    fflush(stdout);
    fprintf(stderr, "\nAt: %s:%zu\nError: %s\n", file, line, message);
    exit(1);
}

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
__MutableOption_expect(const char *__file, size_t __line, MutableOption self, const char *message) {
    assert(message);
    assert(__file);
    if (MutableOption_isNone(self)) {
        __MutableOption_panic(__file, __line, message);
    }
    return self.__some;
}

void *
__MutableOption_unwrap(const char *__file, size_t __line, MutableOption self) {
    assert(__file);
    return __MutableOption_expect(__file, __line, self, "Unable to unwrap value.");
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
