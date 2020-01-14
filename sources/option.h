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
#define __OPTION_SOME_TAG    0xA5
#define __OPTION_NONE_TAG    0x5A

/**
 * An option-type or maybe-type is a polymorphic type that represents encapsulation of an optional value;
 * e.g. it is used as the return type of functions which may or may not return a meaningful value when they are applied.
 */

/**
 * Macro used to generate declarations of the option type (usually used in .h files).
 *
 * @param Identifier is the name of the generated option type.
 * @param Type is the type of the wrapped value.
 * @attention the struct must be treated as opaque therefore its members must not be accessed directly, use the generated functions instead.
 */
#define OptionDeclare(Identifier, Type)                                                                                 \
    struct Identifier { Type __value; unsigned char __tag; };                                                           \
    \
    extern struct Identifier Identifier##_some(Type value)                                                              \
    __attribute__((__warn_unused_result__));                                                                            \
    \
    extern struct Identifier Identifier##_none(void)                                                                    \
    __attribute__((__warn_unused_result__));                                                                            \
    \
    extern bool Identifier##_isSome(struct Identifier self)                                                             \
    __attribute__((__warn_unused_result__));                                                                            \
    \
    extern bool Identifier##_isNone(struct Identifier self)                                                             \
    __attribute__((__warn_unused_result__));                                                                            \
    \
    extern Type Identifier##_unwrap(struct Identifier self)                                                             \
    __attribute__((__warn_unused_result__));                                                                            \
    \
    extern Type Identifier##_expect(struct Identifier self, const char *fmt, ...)                                       \
    __attribute__((__warn_unused_result__, __nonnull__(2), __format__(__printf__, 2, 3))) /* semi-colon */

/**
 * Macro used to generate definitions of the option type (usually used in .c files).
 *
 * @param Identifier is the name of the generated option type.
 * @param Type is the type of the wrapped value.
 */
#define OptionDefine(Identifier, Type)                                                                                  \
    struct Identifier Identifier##_some(Type value) {                                                                   \
        return (struct Identifier) { .__value = value, .__tag = __OPTION_SOME_TAG };                                    \
    }                                                                                                                   \
    \
    struct Identifier Identifier##_none(void) {                                                                         \
        return (struct Identifier) { .__tag = __OPTION_NONE_TAG };                                                      \
    }                                                                                                                   \
    \
    bool Identifier##_isSome(const struct Identifier self) {                                                            \
        return __OPTION_SOME_TAG == self.__tag;                                                                         \
    }                                                                                                                   \
    \
    bool Identifier##_isNone(const struct Identifier self) {                                                            \
        return __OPTION_NONE_TAG == self.__tag;                                                                         \
    }                                                                                                                   \
    \
    Type Identifier##_unwrap(const struct Identifier self) {                                                            \
        if (__OPTION_SOME_TAG == self.__tag) { return self.__value; }                                                   \
        else                                 { panic("unable to unwrap value"); }                                       \
    }                                                                                                                   \
    \
    Type Identifier##_expect(const struct Identifier self, const char *const fmt, ...) {                                \
        assert(NULL != fmt);                                                                                            \
        if (__OPTION_SOME_TAG == self.__tag) { return self.__value; }                                                   \
        else                                 { va_list args; va_start(args, fmt); __vpanic(__TRACE__, fmt, args); }     \
    }                                                                                                                   \
    \
    typedef int __option_##Identifier##_defined__ /* semi-colon */

#ifdef __cplusplus
}
#endif
