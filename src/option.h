/*
 * C Header File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   October 31, 2017 
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

#include <stdbool.h>

#ifndef OPTION_INCLUDED
#define OPTION_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#if !(defined(__GNUC__) || defined(__clang__))
#define __attribute__(...)
#endif

/**
 * This function will terminate the program printing an error message on stderr.
 *
 * @param message The message to be printed.
 * @param file The file name.
 * @param line The line number.
 */
extern void __Option_panic(const char *message, const char *file, size_t line) __attribute__((__noreturn__));

/**
 * Option_T type definitions, must be treated as opaque.
 */
typedef struct Option_T {
    void *const __some;
} Option;

/**
 * The None option instance
 */
extern const Option Option_None;

/**
 * Just for type hinting
 */
#define Optional(Type) Option

/**
 * Creates an Option starting from some value.
 * If @param some is NULL returns Option_None.
 *
 * @param some The option value.
 * @return A new Option instance or Option_None.
 */
extern Option Option_new(void *some);

/**
 * Returns true if the option is a Some value.
 *
 * @param self The Option instance.
 * @return true if is Some value else false.
 */
extern bool Option_isSome(Option self);

/**
 * Returns true if the option is a None value.
 *
 * @param self The Option instance.
 * @return true if is None value else false.
 */
extern bool Option_isNone(Option self);

/**
 * Unwraps an option, yielding the content of a Some.
 * Panics if the value is a None with a custom panic message.
 *
 * @param self The Option instance.
 * @param message The custom error message.
 * @param file The file name.
 * @param line The line number.
 * @return The unwrapped option or terminates the execution.
 */
extern void *__Option_expect(Option self, const char *message, const char *file, size_t line);
#define Option_expect(Self, Message)        __Option_expect((Self), (Message), __FILE__, __LINE__)

/**
 * Unwraps an option, yielding the content of a Some.
 * Panics if the value is a None with a predefined panic message.
 *
 * @param self The Option instance.
 * @param file The file name.
 * @param line The line number.
 * @return The unwrapped option or terminates the execution.
 */
extern void *__Option_unwrap(Option self, const char *file, size_t line);
#define Option_unwrap(Self)                 __Option_unwrap((Self), __FILE__, __LINE__)

/**
 * Maps an Option<T> to Option<U> by applying a function to a contained value.
 *
 * @param self The Option instance.
 * @param mapFn The map function.
 * @return A new Option instance.
 */
extern Option Option_map(Option self, Option mapFn(Option));

/**
 * Applies a function to the contained value (if any), or returns a default (if not).
 *
 * @param self The Option instance.
 * @param def The default option if self is None.
 * @param mapFn The map function.
 * @return A new Option instance.
 */
extern Option Option_mapOr(Option self, Option def, Option mapFn(Option));

/**
 * Applies a function to the contained value (if any), or computes a default (if not).
 *
 * @param self The Option instance.
 * @param def The default function to compute if self is None.
 * @param mapFn The map function.
 * @return A new Option instance.
 */
extern Option Option_mapOrElse(Option self, Option defFn(void), Option mapFn(Option));

#ifdef __cplusplus
}
#endif

#endif /* OPTION_INCLUDED */
