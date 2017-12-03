/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   December 3, 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <option.h>

/*
 *
 */
#define assert_eq(expected, actual)         assert((expected) == (actual))
#define assert_eq_str(expected, actual)     assert_eq(0, strcmp((char *) (expected), (char *) (actual)))

/*
 *
 */
typedef const char *String_T;

static ImmutableOptional(String_T) String_new(const char *s);
static void String_delete(String_T s);

static ImmutableOptional(const char *) Greet(ImmutableOptional(String_T) s);
static ImmutableOptional(const char *) DidNotGetIt(void);

/*
 *
 */
int main() {
    ImmutableOptional(String_T) string = String_new("Hello, World!");

    printf("  Some: %s\n", ImmutableOption_isSome(string) ? "true" : "false");
    printf("  None: %s\n", ImmutableOption_isNone(string) ? "true" : "false");
    printf("string: %s\n", (String_T) ImmutableOption_expect(string, "Needed a string"));

    assert_eq(true, ImmutableOption_isNone(ImmutableOption_new(NULL)));
    assert_eq_str("Hello!", ImmutableOption_unwrap(ImmutableOption_map(ImmutableOption_new("Hi!"), Greet)));
    assert_eq_str("Bla Bla Bla", ImmutableOption_unwrap(ImmutableOption_mapOr(ImmutableOption_None, ImmutableOption_new("Bla Bla Bla"), Greet)));
    assert_eq_str("Oops, didn't get it.", ImmutableOption_unwrap(ImmutableOption_mapOrElse(ImmutableOption_None, DidNotGetIt, Greet)));
    assert_eq_str("Stop bother me!.", ImmutableOption_unwrap(ImmutableOption_mapOrElse(ImmutableOption_None, DidNotGetIt, Greet)));
    assert_eq_str("Goodbye!", ImmutableOption_unwrap(ImmutableOption_map(ImmutableOption_new("Bye!"), Greet)));

    String_delete(ImmutableOption_unwrap(string));
    return EXIT_SUCCESS;
}

/*
 *
 */
ImmutableOptional(String_T) String_new(const char *s) {
    return ImmutableOption_new(strdup(s));
}

void String_delete(String_T s) {
    free((void *) s);
}

ImmutableOptional(const char *) Greet(ImmutableOptional(String_T) s) {
    String_T str = ImmutableOption_unwrap(s);
    if (strcmp(str, "Hi!") == 0) {
        return ImmutableOption_new("Hello!");
    } else if (strcmp(str, "Bye!") == 0) {
        return ImmutableOption_new("Goodbye!");
    }
    return ImmutableOption_None;
}

ImmutableOptional(const char *) DidNotGetIt(void) {
    static int i = -1;
    i++;
    return ImmutableOption_new((i <= 0) ? "Oops, didn't get it." : "Stop bother me!.");
}
