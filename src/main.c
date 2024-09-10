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
            DEBUG_OUT("Not a builtin, pipes\n");
            char *pipe_chain[16] = { NULL };
            char *str = malloc(arg_max);
            strcpy(str, lineptr);
            
            int how_many = separate_pipes(str, pipe_chain, 16);
            
            int in = 0;
            
            for(int i = 0; i < how_many - 1 && pipe_chain[i]; i++) {
                int pipes[2];
                if(pipe(pipes)) abort();          
                
                DEBUG_OUT("Made pipes r:%d w:%d for i = %d\n", pipes[0], pipes[1], i);
                
                str_to_argv(pipe_chain[i], " ", &argv_s);
                char **argv = argv_from_struct(&argv_s);
                spawn_child(argv, in, pipes[1]);

                close(pipes[1]);
                in = pipes[0];
            }
            DEBUG_OUT("On last of chain %s\n", pipe_chain[how_many - 1]);
            str_to_argv(pipe_chain[how_many - 1], " ", &argv_s);
            char **argv = argv_from_struct(&argv_s);
            spawn_child(argv, in, 1);
            
        } else {
            str_to_argv(lineptr, " ", &argv_s);
            char **argv = argv_from_struct(&argv_s);
            if(execute_builtin(argv)) {
                DEBUG_OUT("Finished builtin\n");
            } else {
                DEBUG_OUT("Not a builtin, no pipes\n");
                spawn_child(argv, 0, 1);
            }
            free_str_to_argv(&argv_s);
        }
    }
    
    free(lineptr);
}
