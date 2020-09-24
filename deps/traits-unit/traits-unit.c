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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <trace/trace.h>
#include "traits-unit.h"

/*
 * Some constants
 */
#define traitsUnitStream                stdout
#define TRAITS_UNIT_BUFFER_CAPACITY     1024
#define TRAITS_UNIT_INDENTATION_STEP    2

/*
 * Forward declare traits subject (this should come from the test file Describe macro)
 */
extern struct TraitsUnitSubject traitsUnitSubject;

/*
 * Define private global variables
 */
jmp_buf __traitsUnitJumpBuffer;

/*
 * Define internal global variables
 */
static void *globalContext = NULL;
static bool globalContextInitialized = false;
static const struct TraitsUnitFeature *globalFeature = NULL;
static volatile sig_atomic_t globalWrappedSignalsCounter = 0;
static struct sigaction globalDefaultSignalHandler = {.sa_handler = SIG_DFL};

/*
 * Internals
 */
struct TraitsUnitTraitResult {
    size_t succeed;
    size_t skipped;
    size_t failed;
    size_t all;
};

enum TraitsUnitFeatureResult {
    TRAITS_UNIT_FEATURE_RESULT_SUCCEED,
    TRAITS_UNIT_FEATURE_RESULT_SKIPPED,
    TRAITS_UNIT_FEATURE_RESULT_FAILED,
};

struct Buffer {
    size_t capacity;
    size_t cursor;
    char *content;
};

static struct Buffer *Buffer_new(size_t capacity)
__attribute__((__warn_unused_result__));

static void Buffer_read(struct Buffer *self, int fd)
__attribute__((__nonnull__));

static char *Buffer_get(const struct Buffer *self)
__attribute__((__warn_unused_result__, __nonnull__));

static void Buffer_clear(struct Buffer *self)
__attribute__((__nonnull__));

static void Buffer_delete(struct Buffer *self);


static void *traitsUnit_smalloc(size_t size)
__attribute__((__warn_unused_result__));

static void traitsUnit_sfree(void *address, size_t size);

static void __traitsUnit_panic(const char *trace, const char *fmt, ...)
__attribute__((__noreturn__, __nonnull__, __format__(__printf__, 2, 3)));

static void traitsUnit_print(size_t indentationLevel, const char *fmt, ...)
__attribute__((__nonnull__, __format__(__printf__, 2, 3)));

static void traitsUnit_teardown(void);

static void traitsUnit_registerTeardown(void);

static struct TraitsUnitTraitResult
traitsUnit_runTrait(const struct TraitsUnitTrait *trait, struct Buffer *buffer, size_t indentationLevel)
__attribute__((__nonnull__));

static int traitsUnit_forkAndRunFeature(const struct TraitsUnitFeature *feature, struct Buffer *buffer)
__attribute__((__nonnull__));

static enum TraitsUnitFeatureResult traitsUnit_runFeature(const struct TraitsUnitFeature *feature, struct Buffer *buffer, size_t indentationLevel)
__attribute__((__nonnull__));

static void traitsUnit_report(size_t indentationLevel, size_t succeed, size_t skipped, size_t failed, size_t all);

static void traitsUnit_signalHandler(int signalId);

/*
 * Define internal macros
 */
#define traitsUnit_panic(fmt, ...)  do { __traitsUnit_panic(TRACE, fmt, __VA_ARGS__); abort(); } while(false)

/*
 * Define public functions
 */
void *traitsUnit_getContext(void) {
    if (!globalContextInitialized) {
        traitsUnit_panic("Unexpected call to %s outside call-cycle scope\r\n", __func__);
    }
    return globalContext;
}

size_t traitsUnit_getWrappedSignalsCounter(void) {
    if (globalWrappedSignalsCounter < 0) {
        // this should never happen
        traitsUnit_panic("Unexpected wrapped signals counter value: %ld\r\n", (long) globalWrappedSignalsCounter);
    }
    return (size_t) globalWrappedSignalsCounter;
}

/*
 * Define main
 */
int main(int argc, char *argv[]) {
    struct Buffer *buffer = NULL;
    struct TraitsUnitTrait *traitsList[TRAITS_UNIT_MAX_TRAITS] = {0};
    size_t indentationLevel = 0, succeed = 0, skipped = 0, failed = 0, all = 0;
    bool loaded = true;

    /* Load traitsList */
    if (argc > 1) {
        /* Specific traits must be run */
        if (argc > TRAITS_UNIT_MAX_TRAITS) {
            /* Too many traits has been specified, not able to load traitsList */
            loaded = false;
            traitsUnit_print(indentationLevel, "Too many traits specified\r\n");
        } else {
            /* Search for the specified traits and load them into traitsList */
            size_t index = 0;
            struct TraitsUnitTrait *trait = NULL;

            for (int x = 1; x < argc; x++) {
                bool found = false;

                for (size_t y = 0; (trait = &traitsUnitSubject.traits[y]) && trait->trait; y++) {
                    if (0 == strcmp(trait->trait, argv[x])) {
                        found = true;
                        traitsList[index++] = trait;
                    }
                }

                if (!found) {
                    /* No such trait found, not able to load traitsList */
                    loaded = false;
                    traitsUnit_print(indentationLevel, "Unknown trait: `%s`\r\n", argv[x]);
                    break;
                }
            }
        }
    } else {
        /* Load all traits present in traits_subject */
        for (size_t i = 0; i < TRAITS_UNIT_MAX_TRAITS; i++) {
            traitsList[i] = &traitsUnitSubject.traits[i];
        }
    }

    if (loaded) {
        /* Run features of traits in traitsList */
        struct TraitsUnitTrait *trait = NULL;
        buffer = Buffer_new(TRAITS_UNIT_BUFFER_CAPACITY);
        traitsUnit_print(indentationLevel, "Describing: %s\r\n", traitsUnitSubject.subject);
        indentationLevel += TRAITS_UNIT_INDENTATION_STEP;
        for (size_t i = 0; (trait = traitsList[i]) && trait->trait; i++) {
            struct TraitsUnitTraitResult trait_result = traitsUnit_runTrait(trait, buffer, indentationLevel);
            succeed += trait_result.succeed;
            skipped += trait_result.skipped;
            failed += trait_result.failed;
            all += trait_result.all;
        }
        indentationLevel -= TRAITS_UNIT_INDENTATION_STEP;
        traitsUnit_report(indentationLevel, succeed, skipped, failed, all);
        Buffer_delete(buffer);
    }

    return (loaded && (0 == failed)) ? EXIT_SUCCESS : EXIT_FAILURE;
}

/*
 * Define private functions
 */
struct __TraitsUnitWrapper __TraitsUnitWrapper_new(const int signalId) {
    assert(SIGKILL != signalId && SIGSTOP != signalId);
    struct sigaction *handler = malloc(sizeof(*handler));
    memset(handler, 0, sizeof(*handler));

    if (NULL == handler) {
        traitsUnit_panic("%s\r\n", "Out of memory");
    }

    handler->sa_handler = traitsUnit_signalHandler;
    sigaction(signalId, handler, NULL);
    fflush(traitsUnitStream);

    return (struct __TraitsUnitWrapper) {.pimpl=handler, .signalId=signalId};
}

bool __TraitsUnitWrapper_update(struct __TraitsUnitWrapper *const self) {
    assert(NULL != self);
    if (self->calls++ >= 1) {
        sigaction(self->signalId, &globalDefaultSignalHandler, NULL);
        fflush(traitsUnitStream);
        free(self->pimpl);
        return false;
    } else {
        return true;
    }
}

Setup(__TraitsUnitDefaultSetup) {
    return NULL;
}

Teardown(__TraitsUnitDefaultTeardown) {}

FixtureImplements(__TraitsUnitDefaultFixture, __TraitsUnitDefaultSetup, __TraitsUnitDefaultTeardown);

/*
 * Define internal functions
 */
struct Buffer *Buffer_new(const size_t capacity) {
    struct Buffer *self = traitsUnit_smalloc(sizeof(*self));

    if (NULL == self) {
        traitsUnit_panic("%s\r\n", "Out of memory.");
    }

    self->content = traitsUnit_smalloc(capacity + 1);
    if (NULL == self->content) {
        traitsUnit_panic("%s\r\n", "Out of memory.");
    }

    self->cursor = 0;
    self->capacity = capacity;
    self->content[0] = self->content[capacity] = 0;

    return self;
}

void Buffer_read(struct Buffer *const self, const int fd) {
    assert(NULL != self);
    FILE *stream = fdopen(fd, "r");

    /* Open fd */
    if (NULL == stream) {
        traitsUnit_panic("%s\r\n", "Unable to open file.");
    }

    /* Read from fd and write to self */
    self->cursor += fread(
            self->content + self->cursor,
            sizeof(self->content[0]),
            (self->cursor >= self->capacity) ? 0 : self->capacity - self->cursor,
            stream
    );
    self->content[self->cursor] = 0;

    /* Close the stream */
    fclose(stream);
}

char *Buffer_get(const struct Buffer *const self) {
    assert(NULL != self);
    return self->content;
}

void Buffer_clear(struct Buffer *const self) {
    assert(NULL != self);
    self->cursor = 0;
    self->content[0] = 0;
}

void Buffer_delete(struct Buffer *const self) {
    if (NULL != self) {
        traitsUnit_sfree(self->content, self->capacity + 1);
        traitsUnit_sfree(self, sizeof(*self));
    }
}

void *traitsUnit_smalloc(const size_t size) {
    /* Our memory will be readable and writable */
    const int protection = PROT_READ | PROT_WRITE;

    /*
     * The buffer will be shared (meaning other processes can access it), but
     * anonymous (meaning third-party processes cannot obtain an address for it),
     * so only this process and its children will be able to use it.
     */
    const int visibility = MAP_ANONYMOUS | MAP_SHARED;

    /* Perform memory mapping */
    void *memory = mmap(NULL, size, protection, visibility, 0, 0);
    if (NULL == memory) {
        traitsUnit_panic("%s\n", "Unable to map memory");
    }

    return memory;
}

void traitsUnit_sfree(void *const address, const size_t size) {
    if (0 != munmap(address, size)) {
        traitsUnit_panic("%s\r\n", "Unable to unmap memory");
    }
}

void __traitsUnit_panic(const char *const trace, const char *const fmt, ...) {
    assert(NULL != trace);
    assert(NULL != fmt);
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "\r\nAt %s\r\nError: %s\r\n", trace, strerror(errno));
    vfprintf(stderr, fmt, args);
    fputs("", stderr);
    va_end(args);
    exit(-1);
}

void traitsUnit_print(const size_t indentationLevel, const char *const fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(traitsUnitStream, "%*s", (int) indentationLevel, "");
    vfprintf(traitsUnitStream, fmt, args);
    va_end(args);
}

void traitsUnit_teardown(void) {
    if (NULL == globalFeature) {
        traitsUnit_panic("%s\r\n", "Unexpected error");
    }
    globalFeature->fixture->teardown();
    globalContextInitialized = false;
    globalContext = NULL;
    globalFeature = NULL;
}

void traitsUnit_registerTeardown(void) {
    atexit(traitsUnit_teardown);
}

struct TraitsUnitTraitResult traitsUnit_runTrait(const struct TraitsUnitTrait *const trait, struct Buffer *const buffer, size_t indentationLevel) {
    const struct TraitsUnitFeature *feature = NULL;
    struct TraitsUnitTraitResult traitResult;
    memset(&traitResult, 0, sizeof(traitResult));
    traitsUnit_print(indentationLevel, "Trait: %s\r\n", trait->trait);
    indentationLevel += TRAITS_UNIT_INDENTATION_STEP;
    for (size_t i = 0; (feature = &trait->features[i]) && feature->call && feature->feature; i++) {
        enum TraitsUnitFeatureResult featureResult = traitsUnit_runFeature(feature, buffer, indentationLevel);
        switch (featureResult) {
            case TRAITS_UNIT_FEATURE_RESULT_SUCCEED: {
                traitResult.succeed++;
                break;
            }
            case TRAITS_UNIT_FEATURE_RESULT_SKIPPED: {
                traitResult.skipped++;
                break;
            }
            case TRAITS_UNIT_FEATURE_RESULT_FAILED: {
                traitResult.failed++;
                break;
            }
            default: {
                abort();
            }
        }
        traitResult.all++;
    }
    return traitResult;
}

int traitsUnit_forkAndRunFeature(const struct TraitsUnitFeature *const feature, struct Buffer *const buffer) {
    pid_t pid;
    int fd, pipeFd[2], pidStatus;

    /* Flush traitsUnitStream */
    fflush(traitsUnitStream);

    /* Open a pipe */
    if (pipe(pipeFd) < 0) {
        traitsUnit_panic("%s\r\n", "Unable to open pipe.");
    }

    /* Fork the process */
    if ((pid = fork()) < 0) {
        traitsUnit_panic("%s\r\n", "Unable to fork process.");
    }

    /* We are in the child process */
    if (0 == pid) {
        /* Close read end of pipe */
        close(pipeFd[0]);

        /* Get write end of pipe */
        fd = pipeFd[1];

        /* Redirect STDERR to pipe*/
        dup2(fd, STDERR_FILENO);

        /* Setup globals */
        globalFeature = feature;
        globalContext = feature->fixture->setup();
        globalContextInitialized = true;

        /* Teardown globals on exit */
        traitsUnit_registerTeardown();

        /* Run call */
        feature->call();

        /* Close fd */
        close(fd);

        /* Exit normally */
        exit(EXIT_SUCCESS);
    }

    /* We are in the parent process */

    /* Wait for children */
    wait(&pidStatus);

    /* Close write end of pipe */
    close(pipeFd[1]);

    /* Get read end of pipe */
    fd = pipeFd[0];

    /* Redirect the children output to the global buffer */
    Buffer_read(buffer, fd);

    /* Close fd */
    close(fd);

    /* Flush traitsUnitStream */
    fflush(traitsUnitStream);
    return pidStatus;
}

enum TraitsUnitFeatureResult traitsUnit_runFeature(const struct TraitsUnitFeature *const feature, struct Buffer *const buffer, const size_t indentationLevel) {
    enum TraitsUnitFeatureResult featureResult;
    traitsUnit_print(indentationLevel, "Feature: %s... ", feature->feature);

    if (feature->skip) {
        featureResult = TRAITS_UNIT_FEATURE_RESULT_SKIPPED;
        traitsUnit_print(0, "skipped\r\n");
    } else {
        Buffer_clear(buffer);
        const int exit_status = traitsUnit_forkAndRunFeature(feature, buffer);
        if (EXIT_SUCCESS == exit_status) {
            featureResult = TRAITS_UNIT_FEATURE_RESULT_SUCCEED;
            traitsUnit_print(0, "succeed\r\n");
        } else {
            featureResult = TRAITS_UNIT_FEATURE_RESULT_FAILED;
            if (!WIFEXITED(exit_status)) {
                if (WIFSIGNALED(exit_status)) {
                    traitsUnit_print(
                            0, "(terminated by signal %d - %s) ",
                            WTERMSIG(exit_status), strsignal(WTERMSIG(exit_status))
                    );
                } else {
                    traitsUnit_print(0, "(terminated abnormally) ");
                }
            }
            traitsUnit_print(0, "failed\r\n\r\n%s\r\n", Buffer_get(buffer));
        }
    }

    return featureResult;
}

void traitsUnit_report(const size_t indentationLevel, const size_t succeed, const size_t skipped, const size_t failed, const size_t all) {
    const int width = snprintf(NULL, 0, "%zu", all);
    traitsUnit_print(0, "\r\n");
    traitsUnit_print(indentationLevel, "Succeed: %*zu\r\n", width, succeed);
    traitsUnit_print(indentationLevel, "Skipped: %*zu\r\n", width, skipped);
    traitsUnit_print(indentationLevel, " Failed: %*zu\r\n", width, failed);
    traitsUnit_print(indentationLevel, "    All: %*zu\r\n", width, all);
}

void traitsUnit_signalHandler(const int signalId) {
    (void) signalId;
    fflush(traitsUnitStream);
    globalWrappedSignalsCounter += 1;
    siglongjmp(__traitsUnitJumpBuffer, 1);
}
