#pragma once
#include <wordexp.h>
struct argv_str {
    wordexp_t wexp;
};

int spawn_child(char **argv, int in, int out);
int separate_pipes(char *str, char **target, size_t sz);
int str_to_argv(const char *str, const char *delim, struct argv_str *argv);
int free_str_to_argv(struct argv_str *argv);
char **argv_from_struct(struct argv_str *argv);

void write_ps();
long int read_line(char *lineptr, long int n);
