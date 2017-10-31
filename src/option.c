/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   October 31, 2017 
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "option.h"

void __Option_panic(const char *message, const char *file, size_t line) {
    fflush(stdout);
    fprintf(stderr, "\nAt: %s:%zu\nError: %s\n", file, line, message);
    exit(1);
}

const Option Option_None = {.__some=NULL};

Option Option_new(void *value) {
    Option self = (value) ? (Option) {
            .__some=value
    } : Option_None;
    return self;
}

bool Option_isSome(Option self) {
    return NULL != self.__some;
}

bool Option_isNone(Option self) {
    return NULL == self.__some;
}

void *__Option_expect(Option self, const char *message, const char *file, size_t line) {
    assert(message);
    assert(file);
    if (Option_isNone(self)) {
        __Option_panic(message, file, line);
    }
    return self.__some;
}

void *__Option_unwrap(Option self, const char *file, size_t line) {
    assert(file);
    if (Option_isNone(self)) {
        __Option_panic("Unable to unwrap value.", file, line);
    }
    return self.__some;
}

Option Option_map(Option self, Option mapFn(Option)) {
    return mapFn(self);
}

Option Option_mapOr(Option self, Option def, Option mapFn(Option)) {
    return Option_isSome(self) ? mapFn(self) : def;
}

Option Option_mapOrElse(Option self, Option defFn(void), Option mapFn(Option)) {
    return Option_isSome(self) ? mapFn(self) : defFn();
}
