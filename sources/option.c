/*
Author: daddinuz
email:  daddinuz@gmail.com

Copyright (c) 2018 Davide Di Carlo

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
 */

#include <assert.h>
#include <stddef.h>
#include <panic/panic.h>
#include "option.h"

const Option None = {.__value=NULL};

Option Option_some(const void *value) {
    Panic_when(NULL == value);
    return (Option) {.__value=value};
}

Option Option_fromNullable(const void *value) {
    return NULL == value ? None : Option_some(value);
}

bool Option_isNone(const Option self) {
    return NULL == self.__value;
}

bool Option_isSome(const Option self) {
    return NULL != self.__value;
}

Option Option_map(const Option self, const void *(*const f)(const void *)) {
    Panic_when(NULL == f);
    return Option_isNone(self) ? self : Option_fromNullable(f(Option_unwrap(self)));
}

Option Option_chain(const Option self, Option (*const f)(const void *)) {
    Panic_when(NULL == f);
    return Option_isNone(self) ? self : f(Option_unwrap(self));
}

Option Option_alt(const Option self, const Option other) {
    return Option_isSome(self) ? self : other;
}

Option Option_orElse(const Option self, Option (*const f)(void)) {
    Panic_when(NULL == f);
    return Option_isSome(self) ? self : f();
}

const void *__Option_unwrap(const char *const file, const int line, const Option self) {
    assert(NULL != file);
    if (Option_isNone(self)) {
        __Panic_terminate(file, line, "%s", "Unable to unwrap value");
    }
    return self.__value;
}

void *__Option_unwrapAsMutable(const char *const file, const int line, const Option self) {
    assert(NULL != file);
    if (Option_isNone(self)) {
        __Panic_terminate(file, line, "%s", "Unable to unwrap value");
    }
    return (void *) self.__value;
}

const void *__Option_expect(const char *const file, const int line, const Option self, const char *const format, ...) {
    assert(NULL != file);
    assert(NULL != format);
    if (Option_isNone(self)) {
        va_list args;
        va_start(args, format);
        __Panic_vterminate(file, line, format, args);
    }
    return self.__value;
}

void *
__Option_expectAsMutable(const char *const file, const int line, const Option self, const char *const format, ...) {
    assert(NULL != file);
    assert(NULL != format);
    if (Option_isNone(self)) {
        va_list args;
        va_start(args, format);
        __Panic_vterminate(file, line, format, args);
    }
    return (void *) self.__value;
}
