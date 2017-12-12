/*
 * C Header File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   December 3, 2017
 */

/*
 * Option type trying to resemble Rust's Option.
 * Here Option is basically a wrapper around NULL values
 * in order make safer code avoiding to directly deref
 * pointers without NULL checking and getting segfaults.
 *
 * Any function or member that starts with underscores
 * should be treated as private and should not be used directly.
 */

#ifndef OPTION_INCLUDED
#define OPTION_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#define OPTION_VERSION          "0.3.0"
#define OPTION_VERSION_MAJOR    0
#define OPTION_VERSION_MINOR    3
#define OPTION_VERSION_PATCH    0

#include <stddef.h>
#include <stdbool.h>

#if !(defined(__GNUC__) || defined(__clang__))
#define __attribute__(...)
#endif

/*
 * Mutable Option
 */

/**
 * MutableOption type should never be used directly.
 * Must be treated as opaque.
 */
typedef struct MutableOption_T {
    void *__some;
} MutableOption;

/**
 * Just for type hinting
 */
#define MutableOptional(Type) MutableOption

/**
 * The None MutableOption instance
 */
extern const MutableOption MutableOption_None;

/**
 * Creates an MutableOption starting from some value.
 * If @param some is NULL returns MutableOption_None.
 *
 * @param some The option value.
 * @return A new MutableOption instance or MutableOption_None.
 */
extern MutableOption
MutableOption_new(void *some);

/**
 * Returns true if the option is a Some value.
 *
 * @param self The MutableOption instance.
 * @return true if is Some value else false.
 */
extern bool
MutableOption_isSome(MutableOption self);

/**
 * Returns true if the option is a None value.
 *
 * @param self The MutableOption instance.
 * @return true if is None value else false.
 */
extern bool
MutableOption_isNone(MutableOption self);

/**
 * Unwraps an option, yielding the content of a Some.
 * Panics if the value is a None with a custom panic message.
 *
 * @param __file The file name.
 * @param __line The line number.
 * @param self The MutableOption instance.
 * @param format The custom error message.
 * @param ... The format params.
 * @return The unwrapped option or terminates the execution.
 */
extern void *
__MutableOption_expect(const char *__file, size_t __line, MutableOption self, const char *format, ...)
__attribute__((__nonnull__(1, 4), __format__(__printf__, 4, 5)));

#define MutableOption_expect(Self, ...) \
    __MutableOption_expect(__FILE__, __LINE__, (Self), __VA_ARGS__)

/**
 * Unwraps an option, yielding the content of a Some.
 * Panics if the value is a None with a predefined panic message.
 *
 * @param __file The file name.
 * @param __line The line number.
 * @param self The MutableOption instance.
 * @return The unwrapped option or terminates the execution.
 */
extern void *
__MutableOption_unwrap(const char *__file, size_t __line, MutableOption self)
__attribute__((__nonnull__(1)));

#define MutableOption_unwrap(Self)      \
    __MutableOption_unwrap(__FILE__, __LINE__, (Self))

/**
 * Maps an MutableOption<T> to MutableOption<U> by applying a function to a contained value.
 *
 * Checked runtime errors:
 *      - @param mapFn must not be NULL.
 *
 * @param self The MutableOption instance.
 * @param mapFn The map function.
 * @return A new MutableOption instance.
 */
extern MutableOption
MutableOption_map(MutableOption self, MutableOption mapFn(MutableOption))
__attribute__((__nonnull__));

/**
 * Applies a function to the contained value (if any), or returns a default (if not).
 *
 * Checked runtime errors:
 *      - @param mapFn must not be NULL.
 *
 * @param self The MutableOption instance.
 * @param def The default option if self is None.
 * @param mapFn The map function.
 * @return A new MutableOption instance.
 */
extern MutableOption
MutableOption_mapOr(MutableOption self, MutableOption def, MutableOption mapFn(MutableOption))
__attribute__((__nonnull__));

/**
 * Applies a function to the contained value (if any), or computes a default (if not).
 *
 * Checked runtime errors:
 *      - @param defFn must not be NULL.
 *      - @param mapFn must not be NULL.
 *
 * @param self The MutableOption instance.
 * @param def The default function to compute if self is None.
 * @param mapFn The map function.
 * @return A new MutableOption instance.
 */
extern MutableOption
MutableOption_mapOrElse(MutableOption self, MutableOption defFn(void), MutableOption mapFn(MutableOption))
__attribute__((__nonnull__));

/*
 * Immutable Option
 */


/**
 * ImmutableOption type should never be used directly.
 * Must be treated as opaque.
 */
typedef struct ImmutableOption_T {
    const void *__some;
} ImmutableOption;

/**
 * Just for type hinting
 */
#define ImmutableOptional(Type) ImmutableOption

/**
 * The None ImmutableOption instance
 */
extern const ImmutableOption ImmutableOption_None;

/**
 * Creates an ImmutableOption starting from some value.
 * If @param some is NULL returns ImmutableOption_None.
 *
 * @param some The option value.
 * @return A new ImmutableOption instance or ImmutableOption_None.
 */
extern ImmutableOption
ImmutableOption_new(const void *some);

/**
 * Returns true if the option is a Some value.
 *
 * @param self The ImmutableOption instance.
 * @return true if is Some value else false.
 */
extern bool
ImmutableOption_isSome(ImmutableOption self);

/**
 * Returns true if the option is a None value.
 *
 * @param self The ImmutableOption instance.
 * @return true if is None value else false.
 */
extern bool
ImmutableOption_isNone(ImmutableOption self);

/**
 * Unwraps an option, yielding the content of a Some.
 * Panics if the value is a None with a custom panic message.
 *
 * @param __file The file name.
 * @param __line The line number.
 * @param self The ImmutableOption instance.
 * @param format The custom error message.
 * @param ... The format params.
 * @return The unwrapped option or terminates the execution.
 */
extern const void *
__ImmutableOption_expect(const char *__file, size_t __line, ImmutableOption self, const char *format, ...)
__attribute__((__nonnull__(1, 4), __format__(__printf__, 4, 5)));

#define ImmutableOption_expect(Self, ...)   \
    __ImmutableOption_expect(__FILE__, __LINE__, (Self), __VA_ARGS__)

/**
 * Unwraps an option, yielding the content of a Some.
 * Panics if the value is a None with a predefined panic message.
 *
 * @param __file The file name.
 * @param __line The line number.
 * @param self The ImmutableOption instance.
 * @return The unwrapped option or terminates the execution.
 */
extern const void *
__ImmutableOption_unwrap(const char *__file, size_t __line, ImmutableOption self)
__attribute__((__nonnull__(1)));

#define ImmutableOption_unwrap(Self)        \
    __ImmutableOption_unwrap(__FILE__, __LINE__, (Self))

/**
 * Maps an ImmutableOption<T> to ImmutableOption<U> by applying a function to a contained value.
 *
 * Checked runtime errors:
 *      - @param mapFn must not be NULL.
 *
 * @param self The ImmutableOption instance.
 * @param mapFn The map function.
 * @return A new ImmutableOption instance.
 */
extern ImmutableOption
ImmutableOption_map(ImmutableOption self, ImmutableOption mapFn(ImmutableOption))
__attribute__((__nonnull__));

/**
 * Applies a function to the contained value (if any), or returns a default (if not).
 *
 * Checked runtime errors:
 *      - @param mapFn must not be NULL.
 *
 * @param self The ImmutableOption instance.
 * @param def The default option if self is None.
 * @param mapFn The map function.
 * @return A new ImmutableOption instance.
 */
extern ImmutableOption
ImmutableOption_mapOr(ImmutableOption self, ImmutableOption def, ImmutableOption mapFn(ImmutableOption))
__attribute__((__nonnull__));

/**
 * Applies a function to the contained value (if any), or computes a default (if not).
 *
 * Checked runtime errors:
 *      - @param defFn must not be NULL.
 *      - @param mapFn must not be NULL.
 *
 * @param self The ImmutableOption instance.
 * @param def The default function to compute if self is None.
 * @param mapFn The map function.
 * @return A new ImmutableOption instance.
 */
extern ImmutableOption
ImmutableOption_mapOrElse(ImmutableOption self, ImmutableOption defFn(void), ImmutableOption mapFn(ImmutableOption))
__attribute__((__nonnull__));

/*
 * C11 Support
 */

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define Option_isSome(x)                    _Generic((x), MutableOption: MutableOption_isSome, ImmutableOption: ImmutableOption_isSome)((x))
#define Option_isNone(x)                    _Generic((x), MutableOption: MutableOption_isNone, ImmutableOption: ImmutableOption_isNone)((x))
#define Option_expect(x, ...)               _Generic((x), MutableOption: __MutableOption_expect, ImmutableOption: __ImmutableOption_expect)(__FILE__, __LINE__, (x), __VA_ARGS__)
#define Option_unwrap(x)                    _Generic((x), MutableOption: __MutableOption_unwrap, ImmutableOption: __ImmutableOption_unwrap)(__FILE__, __LINE__, (x))
#define Option_map(x, mapFn)                _Generic((x), MutableOption: MutableOption_map, ImmutableOption: ImmutableOption_map)((x), (mapFn))
#define Option_mapOr(x, def, mapFn)         _Generic((x), MutableOption: MutableOption_mapOr, ImmutableOption: ImmutableOption_mapOr)((x), (def), (mapFn))
#define Option_mapOrElse(x, defFn, mapFn)   _Generic((x), MutableOption: MutableOption_mapOrElse, ImmutableOption: ImmutableOption_mapOrElse)((x), (defFn), (mapFn))
#endif

#ifdef __cplusplus
}
#endif

#endif /* OPTION_INCLUDED */
