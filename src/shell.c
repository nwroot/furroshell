#include "shell.h"
#include "debug.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

int spawn_child(char **argv, int in, int out) {
    DEBUG_OUT("Spawning child\n");
    int f = fork();
    if(f == 0) {
        DEBUG_OUT("CHILD: execvp(\"%s\", %p)\n", argv[0], &argv);
        
        if(in != 0 || out != 1) {
            DEBUG_OUT("CHILD: Maybe stdin/stdout and replacing with %d/%d, goodbye!\n", in, out);
            
            if(in != 0) {
                dup2(in, 0);
                close(in);
            }
            if(out != 1) {
                dup2(out, 1);
                close(in);
            }
        }
        
        // Child process starts here
        execvp(argv[0], argv);
        
        // Failed
        DEBUG_OUT("CHILD: Execution on child! execlp(): %s\n", strerror(errno));
        fprintf(stderr, "%s: command not found\n", argv[0]);
        
        exit(0);
    } else {
        if(f == -1) {
            DEBUG_OUT("PARENT: Failed to fork(): %s\n", strerror(errno));
            abort();
        }
        DEBUG_OUT("PARENT: Waiting for %d to exit...\n", f);
        waitpid(f, NULL, 0);
        DEBUG_OUT("PARENT: %d exited.\n", f);
    }
    return 0;
}

void write_ps() {
    fprintf(stdout, "> ");
}

int separate_pipes(char *str, char **target, size_t sz) {
    char *state = NULL;
    
    char *current = NULL;
    size_t i = 0;
    
    char *str1 = str;
    while(i < sz && (current = strtok_r(str1, "|", &state))) {
        target[i] = current;
        i++;
        str1 = NULL;
    }
    return i;
}

int str_to_argv(const char *str, const char *delim, struct argv_str *argv) {
    int ret = wordexp(str, &argv->wexp, WRDE_NOCMD);
    if(ret) {
        DEBUG_OUT("wordexp() failed with return code %d\n", ret);
        return ret;
    }
    return 0;
}

int free_str_to_argv(struct argv_str *argv) {
    wordfree(&argv->wexp);
    return 0;
}

char **argv_from_struct(struct argv_str *argv) {
    return argv->wexp.we_wordv;
}

static int is_line_separator(char c) {
    return c == '\n';
}

long int read_line(char *lineptr, long int n) {
    long int i = 0;
    for(i = 0; i < n - 1; i++) {
        int c = getchar();
        if(is_line_separator(c)) {
            lineptr[i] = 0;
            return i;
        }
        if(c == EOF) {
            if(i == 0) return -1;
            lineptr[i] = 0;
            return i;
        }
        lineptr[i] = c;
    }
    DEBUG_OUT("Didn\'t encounter line terminator on read\n");
    lineptr[i + 1] = 0;
    return i + 1;
}
