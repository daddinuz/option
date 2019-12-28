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

#include <stringify/stringify.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <signal.h>
#include <setjmp.h>
#include <stdbool.h>

#if !defined(__GNUC__)
#define __attribute__(...)
#endif

/* [public section begin] */

/*
 * Constants
 */
#define TRAITS_UNIT_MAX_TRAITS      96
#define TRAITS_UNIT_MAX_FEATURES    64

struct TraitsUnitFixture {
    void *(*setup)(void);
    void (*teardown)(void);
};

struct TraitsUnitFeature {
    struct TraitsUnitFixture *fixture;
    const char *feature;
    void (*call)(void);
    bool skip;
};

struct TraitsUnitTrait {
    struct TraitsUnitFeature features[TRAITS_UNIT_MAX_FEATURES];
    const char *trait;
};

struct TraitsUnitSubject {
    struct TraitsUnitTrait traits[TRAITS_UNIT_MAX_TRAITS];
    const char *subject;
};

extern void *traitsUnit_getContext(void)
__attribute__((__warn_unused_result__));

extern size_t traitsUnit_getWrappedSignalsCounter(void)
__attribute__((__warn_unused_result__));

/*
 * Declare main in order to force definition by traits-unit
 */
extern int main(int argc, char *argv[]);

/*
 * Macros
 */
#define Setup(Name)                                 \
    void *__traitsUnit_setupId(Name)(void)

#define Teardown(Name)                              \
    void __traitsUnit_teardownId(Name)(void)

#define Feature(Name)                               \
    void __traitsUnit_featureId(Name)(void)

#define Fixture(Name)                               \
    extern struct TraitsUnitFixture __traitsUnit_fixtureId(Name)

#define FixtureImplements(Name, Setup, Teardown)    \
    struct TraitsUnitFixture __traitsUnit_fixtureId(Name) = {.setup=__traitsUnit_setupId(Setup), .teardown=__traitsUnit_teardownId(Teardown)}

#define Describe(Subject, ...)                      \
    struct TraitsUnitSubject traitsUnitSubject = {.traits={__VA_ARGS__}, .subject=(Subject)};

#define Trait(Name, ...)                            \
    {.features={__VA_ARGS__}, .trait=(Name)}

#define Run(...)                                    \
    __traitsUnit_runFeature(__VA_ARGS__, __TraitsUnitDefaultFixture, __)

#define Skip(...)                                   \
    __traitsUnit_skipFeature(__VA_ARGS__, __TraitsUnitDefaultFixture, __)

/*
 * Helper macro to handle signals
 */
#define traitsUnit_wrap(signalId)                                                       \
    for (                                                                               \
        struct __TraitsUnitWrapper wrapper = __TraitsUnitWrapper_new((signalId));       \
        !sigsetjmp(__traitsUnitJumpBuffer, 1) && __TraitsUnitWrapper_update(&wrapper);  \
    )

/* [public section end] */

/* [private section begin] */

#define __traitsUnit_tokenJoinPhase2(x, ...)    x ## __VA_ARGS__
#define __traitsUnit_tokenJoin(x, ...)          __traitsUnit_tokenJoinPhase2(x, __VA_ARGS__)

#define __traitsUnit_setupId(Name)              __traitsUnit_tokenJoin(traitsUnit_userSetup, Name)
#define __traitsUnit_teardownId(Name)           __traitsUnit_tokenJoin(traitsUnit_userTeardown, Name)
#define __traitsUnit_fixtureId(Name)            __traitsUnit_tokenJoin(traitsUnit_userFixture, Name)
#define __traitsUnit_featureId(Name)            __traitsUnit_tokenJoin(traitsUnit_userFeature, Name)

#define __traitsUnit_runFeature(Name, Fixture, ...)     \
    {.fixture=&__traitsUnit_fixtureId(Fixture), .feature=stringify(Name), .call=__traitsUnit_featureId(Name), .skip=false}

#define __traitsUnit_skipFeature(Name, Fixture, ...)    \
    {.fixture=&__traitsUnit_fixtureId(Fixture), .feature=stringify(Name), .call=__traitsUnit_featureId(Name), .skip=true}

extern jmp_buf __traitsUnitJumpBuffer;

struct __TraitsUnitWrapper {
    void *pimpl;
    int signalId;
    unsigned calls;
};

extern struct __TraitsUnitWrapper __TraitsUnitWrapper_new(int signalId)
__attribute__((__warn_unused_result__));

extern bool __TraitsUnitWrapper_update(struct __TraitsUnitWrapper *self)
__attribute__((__nonnull__));

Fixture(__TraitsUnitDefaultFixture);

/* [private section end] */

#ifdef __cplusplus
}
#endif
