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

const Option None = {.__value=NULL, .__variant=OptionNone};

Option Option_some(void *const value) {
    return (Option) {.__value=value, .__variant=OptionSome};
}

Option Option_fromNullable(void *const value) {
    return NULL == value ? None : Option_some(value);
}

bool Option_isNone(const Option self) {
    return OptionNone == self.__variant;
}

bool Option_isSome(const Option self) {
    return OptionSome == self.__variant;
}

Option Option_map(const Option self, Option (*const f)(void *)) {
    assert(NULL != f);
    return Option_isNone(self) ? None : f(Option_unwrap(self));
}

Option Option_alt(const Option self, const Option a) {
    return Option_isNone(self) ? a : self;
}

Option Option_chain(const Option self, Option (*const f)(void *)) {
    assert(NULL != f);
    return Option_isNone(self) ? None : f(Option_unwrap(self));
}

void *Option_fold(const Option self, void *(*const whenNone)(void), void *(*const whenSome)(void *)) {
    assert(NULL != whenNone);
    assert(NULL != whenSome);
    return Option_isNone(self) ? whenNone() : whenSome(Option_unwrap(self));
}

void *Option_getOr(const Option self, void *const defaultValue) {
    return Option_isNone(self) ? defaultValue : Option_unwrap(self);
}

void *Option_getOrElse(const Option self, void *(*const f)(void)) {
    assert(NULL != f);
    return Option_isNone(self) ? f() : Option_unwrap(self);
}

void *__Option_unwrap(const char *const file, const int line, const Option self) {
    assert(NULL != file);
    assert(line > 0);
    if (Option_isNone(self)) {
        __Panic_terminate(file, line, "%s", "Unable to unwrap value");
    }
    return self.__value;
}

void *__Option_expect(const char *const file, const int line, const Option self, const char *const format, ...) {
    assert(NULL != file);
    assert(line > 0);
    assert(NULL != format);
    if (Option_isNone(self)) {
        va_list args;
        va_start(args, format);
        __Panic_vterminate(file, line, format, args);
    }
    return self.__value;
}
