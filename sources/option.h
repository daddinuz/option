/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Davide Di Carlo
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
#include <stdbool.h>

#if !defined(__GNUC__)
#define __attribute__(...)
#endif

// internal use only, must not be used directly.
typedef unsigned char __OptionTag;

#define __OPTION_SOME_TAG    0xA5u
#define __OPTION_NONE_TAG    0x5Au

/**
 * An option-type or maybe-type is a polymorphic type that represents encapsulation of an optional value;
 * e.g. it is used as the return type of functions which may or may not return a meaningful value when they are applied.
 */

/**
 * Macro used to generate declarations of the option type (usually used in .h files).
 *
 * @param Struct is the name of the generated option type.
 * @param Type is the type of the wrapped value.
 * @attention the struct must be treated as opaque therefore its members must not be accessed directly, use the generated functions instead.
 */
#define OptionDeclare(Struct, Type)                                                                                     \
    struct Struct { Type __value; __OptionTag __tag; };                                                                 \
    \
    extern struct Struct Struct##_some(Type value)                                                                      \
    __attribute__((__warn_unused_result__));                                                                            \
    \
    extern struct Struct Struct##_none(void)                                                                            \
    __attribute__((__warn_unused_result__));                                                                            \
    \
    extern bool Struct##_isSome(const struct Struct *self)                                                              \
    __attribute__((__warn_unused_result__, __nonnull__(1)));                                                            \
    \
    extern bool Struct##_isNone(const struct Struct *self)                                                              \
    __attribute__((__warn_unused_result__, __nonnull__(1)));                                                            \
    \
    extern Type Struct##_unwrap(const struct Struct *self)                                                              \
    __attribute__((__warn_unused_result__, __nonnull__(1)));                                                            \
    \
    extern Type Struct##_expect(const struct Struct *self, const char *fmt, ...)                                        \
    __attribute__((__warn_unused_result__, __nonnull__(1, 2), __format__(__printf__, 2, 3))) /* semi-colon */

/**
 * Macro used to generate definitions of the option type (usually used in .c files).
 *
 * @param Struct is the name of the generated option type.
 * @param Type is the type of the wrapped value.
 */
#define OptionDefine(Struct, Type)                                                                                      \
    struct Struct Struct##_some(Type value) {                                                                           \
        return (struct Struct) { .__value = value, .__tag = __OPTION_SOME_TAG };                                        \
    }                                                                                                                   \
    \
    struct Struct Struct##_none(void) {                                                                                 \
        return (struct Struct) { .__tag = __OPTION_NONE_TAG };                                                          \
    }                                                                                                                   \
    \
    bool Struct##_isSome(const struct Struct *const self) {                                                             \
        assert(NULL != self);                                                                                           \
        return __OPTION_SOME_TAG == self->__tag;                                                                        \
    }                                                                                                                   \
    \
    bool Struct##_isNone(const struct Struct *const self) {                                                             \
        assert(NULL != self);                                                                                           \
        return __OPTION_NONE_TAG == self->__tag;                                                                        \
    }                                                                                                                   \
    \
    Type Struct##_unwrap(const struct Struct *const self) {                                                             \
        assert(NULL != self);                                                                                           \
        if (__OPTION_SOME_TAG == self->__tag) { return self->__value; }                                                 \
        else                                 { panic("unable to unwrap value"); }                                       \
    }                                                                                                                   \
    \
    Type Struct##_expect(const struct Struct *const self, const char *const fmt, ...) {                                 \
        assert(NULL != self);                                                                                           \
        assert(NULL != fmt);                                                                                            \
        if (__OPTION_SOME_TAG == self->__tag) { return self->__value; }                                                 \
        else                                 { va_list args; va_start(args, fmt); __vpanic(__TRACE__, fmt, args); }     \
    }                                                                                                                   \
    \
    static_assert(1, "") /* semi-colon */

#ifdef __cplusplus
}
#endif
