/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   December 12, 2017 
 */

#include <stdio.h>
#include <string.h>
#include <option.h>

#define truth(x)    ((x) ? "true" : "false")

/*
 *
 */
static MutableOption mutableMapFn(MutableOption option);
static MutableOption mutableDefFn(void);

static ImmutableOption immutableMapFn(ImmutableOption option);
static ImmutableOption immutableDefFn(void);

/*
 *
 */
int main() {
    MutableOptional(char *) mutableOption = MutableOption_new(strdup("Mutable String"));
    ImmutableOptional(const char *) immutableOption = ImmutableOption_new("Immutable String");

    puts("MutableOption:");
    printf("\tisSome: %s\n", truth(Option_isSome(mutableOption)));
    printf("\tisNone: %s\n", truth(Option_isNone(mutableOption)));
    printf("\texpect: %s\n", (char *) Option_expect(mutableOption, "%s", "Expected a string."));
    printf("\tmap: %s\n", (char *) Option_unwrap(Option_map(MutableOption_None, mutableMapFn)));
    printf("\tmapOr: %s\n", (char *) Option_unwrap(Option_mapOr(MutableOption_None, MutableOption_new(strdup("Mutable Default String")), mutableMapFn)));
    printf("\tmapOrElse: %s\n", (char *) Option_unwrap(Option_mapOrElse(MutableOption_None, mutableDefFn, mutableMapFn)));

    puts("ImmutableOption:");
    printf("\tisSome: %s\n", truth(Option_isSome(immutableOption)));
    printf("\tisNone: %s\n", truth(Option_isNone(immutableOption)));
    printf("\texpect: %s\n", (const char *) Option_expect(immutableOption, "%s", "Expected a string."));
    printf("\tmap: %s\n", (const char *) Option_unwrap(Option_map(ImmutableOption_None, immutableMapFn)));
    printf("\tmapOr: %s\n", (const char *) Option_unwrap(Option_mapOr(ImmutableOption_None, ImmutableOption_new("Immutable Default String"), immutableMapFn)));
    printf("\tmapOrElse: %s\n", (const char *) Option_unwrap(Option_mapOrElse(ImmutableOption_None, immutableDefFn, immutableMapFn)));

    return 0;
}

/*
 * 
 */
MutableOption mutableMapFn(MutableOption option) {
    (void) option;
    static int counter = 1;
    return counter-- > 0 ? MutableOption_new(strdup("Mutable Mapped String")) : MutableOption_None;
}

MutableOption mutableDefFn(void) {
    return MutableOption_new(strdup("Mutable Default Mapped String"));
}

ImmutableOption immutableMapFn(ImmutableOption option) {
    (void) option;
    static int counter = 1;
    return counter-- > 0 ? ImmutableOption_new("Immutable Mapped String") : ImmutableOption_None;
}

ImmutableOption immutableDefFn(void) {
    return ImmutableOption_new("Immutable Default Mapped String");
}
