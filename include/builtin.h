#pragma once

#include "builtins.h"

struct builtin {
    // Name for the builtin
    const char *name;
    // Function that executes the builtin
    int (*exec)(char **);
};

struct builtin builtins[] = {
    { .name = "cd", .exec = &builtin_cd },
    { .name = "pwd", .exec = &builtin_pwd },
    { .name = "exit", .exec = &builtin_exit },
#ifdef HOMEWORK
    { .name = "favs", .exec = &builtin_favs },
#endif
};

const int builtins_sz = sizeof(builtins) / sizeof(struct builtin);
