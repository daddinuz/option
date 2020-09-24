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

#pragma once

#include <panic/panic.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdalign.h>

#if !(defined(__GNUC__) || defined(__clang__))
#define __attribute__(...)
#endif

/**
 * An option-type or maybe-type is a polymorphic type that represents encapsulation of an optional value;
 * e.g. it is used as the return type of functions which may or may not return a meaningful value when they are applied.
 */

/**
 * Macro used to generate declarations of the option type (must be used in .h files).
 *
 * @param Ident is the name of the generated option type.
 * @param Type is the type of the wrapped value.
 * @attention the struct must be treated as opaque therefore its members must not be accessed directly, use the generated functions instead.
 */
#define option_declare(Ident, Type)                                                                                     \
    typedef struct Ident {                                                                                              \
        alignas(Type) char _[sizeof(Type) + 1];                                                                         \
    } Ident;                                                                                                            \
    \
    extern struct Ident Ident##_some(Type value)                                                                        \
    __attribute__((__warn_unused_result__));                                                                            \
    \
    extern struct Ident Ident##_none(void)                                                                              \
    __attribute__((__warn_unused_result__));                                                                            \
    \
    extern bool Ident##_isSome(const struct Ident *self)                                                                \
    __attribute__((__warn_unused_result__, __nonnull__(1)));                                                            \
    \
    extern bool Ident##_isNone(const struct Ident *self)                                                                \
    __attribute__((__warn_unused_result__, __nonnull__(1)));                                                            \
    \
    extern Type Ident##_unwrap(const struct Ident *self)                                                                \
    __attribute__((__warn_unused_result__, __nonnull__(1)));                                                            \
    \
    extern Type Ident##_expect(const struct Ident *self, const char *fmt, ...)                                          \
    __attribute__((__warn_unused_result__, __nonnull__(1, 2), __format__(__printf__, 2, 3)));                           \
    \
    static_assert(1, "") /* semi-colon */

/**
 * Macro used to generate definitions of the option type (must be used in .c files).
 *
 * @param Ident is the name of the generated option type.
 * @param Type is the type of the wrapped value.
 */
#define option_define(Ident, Type)                                                                                      \
    struct Ident Ident##_some(Type value) {                                                                             \
        struct Ident self;                                                                                              \
        memcpy(self._, &value, sizeof(Type));                                                                           \
        self._[sizeof(Type)] = +42;                                                                                     \
        return self;                                                                                                    \
    }                                                                                                                   \
    \
    struct Ident Ident##_none(void) {                                                                                   \
        return (struct Ident) { ._[sizeof(Type)] = -42 };                                                               \
    }                                                                                                                   \
    \
    bool Ident##_isSome(const struct Ident *const self) {                                                               \
        assert(NULL != self);                                                                                           \
        return self->_[sizeof(Type)] == +42;                                                                            \
    }                                                                                                                   \
    \
    bool Ident##_isNone(const struct Ident *const self) {                                                               \
        assert(NULL != self);                                                                                           \
        return self->_[sizeof(Type)] == -42;                                                                            \
    }                                                                                                                   \
    \
    Type Ident##_unwrap(const struct Ident *const self) {                                                               \
        assert(NULL != self);                                                                                           \
        panic_assert(Ident##_isSome(self));                                                                             \
        Type out;                                                                                                       \
        memcpy(&out, self->_, sizeof(Type));                                                                            \
        return out;                                                                                                     \
    }                                                                                                                   \
    \
    Type Ident##_expect(const struct Ident *const self, const char *const fmt, ...) {                                   \
        assert(NULL != self);                                                                                           \
        assert(NULL != fmt);                                                                                            \
        if (Ident##_isSome(self)) { return Ident##_unwrap(self); }                                                      \
        else                      { va_list args; va_start(args, fmt); panic_vabort(TRACE, fmt, args); }                \
    }                                                                                                                   \
    \
    static_assert(1, "") /* semi-colon */

#ifdef __cplusplus
}
#endif
