#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

extern char **environ;

#include "builtins.h"
#include "debug.h"
#include "shell.h"

int main(int argc, char **argv) {
    bool running = true;
    
    long int arg_max = sysconf(_SC_ARG_MAX);
    char *lineptr = malloc(arg_max);

    DEBUG_OUT("ARG_MAX = %ld\n", arg_max);
    if(!lineptr) {
        DEBUG_OUT("Failed to allocated ARG_MAX bytes\n");
    }
    
    while(running) {
        struct argv_str argv_s;
        write_ps();
        long int rd = read_line(lineptr, arg_max);
        if(rd < 0) {
            DEBUG_OUT("rd = %ld\n", rd);
            running = false;
            continue;
        }
        
        if(lineptr[0] == '\n' || lineptr[0] == 0) {
            continue;
        }
        
        if(strchr(lineptr, '|')) {
            char *pipe_chain[16] = { NULL };
            char *str = malloc(strlen(lineptr));
            strcpy(str, lineptr);
            
            separate_pipes(str, pipe_chain, 16);
            
        } else {
            str_to_argv(lineptr, " ", &argv_s);
            char **argv = argv_from_struct(&argv_s);
            if(execute_builtin(argv)) {
                DEBUG_OUT("Finished builtin\n");
            } else {
                DEBUG_OUT("Not a builtin, no pipes case. fork()...\n");
                int f = fork();
                if(f == 0) {
                    DEBUG_OUT("CHILD: execvp(\"%s\", %p)\n", argv[0], &argv);
                    // Child process starts here
                    execvp(argv[0], argv);
                    DEBUG_OUT("CHILD: Execution on child! execlp(): %s\n", strerror(errno));
                    fprintf(stderr, "%s: command not found\n", argv[0]);
                    abort();
                } else {
                    if(f == -1) {
                        DEBUG_OUT("PARENT: Failed to fork(): %s\n", strerror(errno));
                        abort();
                    }
                    DEBUG_OUT("PARENT: Waiting for %d to exit...\n", f);
                    waitpid(f, NULL, 0);
                    DEBUG_OUT("PARENT: %d exited.\n", f);
                }
            }
            free_str_to_argv(&argv_s);
        }
    }
    
    free(lineptr);
}
