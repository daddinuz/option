/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   October 31, 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "option.h"

/*
 *
 */
#define assert_eq(expected, actual)         assert((expected) == (actual))
#define assert_eq_str(expected, actual)     assert_eq(0, strcmp((char *) (expected), (char *) (actual)))

/*
 *
 */
typedef char *String_T;

static Optional(String_T) String_new(const char *s);
static void String_delete(String_T s);

static Optional(const char *) Greet(Optional(String_T) s);
static Optional(const char *) DidNotGetIt(void);

/*
 *
 */
int main() {
    Optional(String_T) string = String_new("Hello, World!");

    printf("  Some: %s\n", Option_isSome(string) ? "true" : "false");
    printf("  None: %s\n", Option_isNone(string) ? "true" : "false");
    printf("string: %s\n", (String_T) Option_expect(string, "Needed a string"));

    assert_eq(true, Option_isNone(Option_new(NULL)));
    assert_eq_str("Hello!", Option_unwrap(Option_map(Option_new("Hi!"), Greet)));
    assert_eq_str("Bla Bla Bla", Option_unwrap(Option_mapOr(Option_None, Option_new("Bla Bla Bla"), Greet)));
    assert_eq_str("Oops, didn't get it.", Option_unwrap(Option_mapOrElse(Option_None, DidNotGetIt, Greet)));
    assert_eq_str("Stop bother me!.", Option_unwrap(Option_mapOrElse(Option_None, DidNotGetIt, Greet)));
    assert_eq_str("Goodbye!", Option_unwrap(Option_map(Option_new("Bye!"), Greet)));

    String_delete(Option_unwrap(string));
    return EXIT_SUCCESS;
}

/*
 *
 */
Optional(String_T) String_new(const char *s) {
    return Option_new(strdup(s));
}

void String_delete(String_T s) {
    free(s);
}

Optional(const char *) Greet(Optional(String_T) s) {
    String_T str = Option_unwrap(s);
    if (strcmp(str, "Hi!") == 0) {
        return Option_new("Hello!");
    } else if (strcmp(str, "Bye!") == 0) {
        return Option_new("Goodbye!");
    }
    return Option_None;
}

Optional(const char *) DidNotGetIt(void) {
    static int i = -1;
    i++;
    return Option_new((i <= 0) ? "Oops, didn't get it." : "Stop bother me!.");
}
