#include "builtin.h"
#include "debug.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>

#define PATH_MAX 4096

int execute_builtin(char **argv) {
    DEBUG_OUT("Searching for builtin %s\n", argv[0]);
    for(int i = 0; i < builtins_sz; i++) {
        if(strcmp(argv[0], builtins[i].name) == 0) {
            DEBUG_OUT("Found %s at %d, executing...\n", argv[0], i);
            builtins[i].exec(argv);
            return 1;
        }
    }
    return 0;
}

int builtin_cd(char **argv) {
    if(!argv[1]) {
        fprintf(stderr, "usage: %s <directory>\n", argv[0]);
        return -1;
    }
    DEBUG_OUT("chdir(\"%s\")\n", argv[1]);
    if(chdir(argv[1])) {
       fprintf(stderr, "chdir() error: %s\n", strerror(errno));
       return -1;
    }
    return 0;
}

int builtin_pwd(char **argv) {
    char cwd[PATH_MAX + 1] = {0};
    // TODO: Paths aren't really PATH_MAX long
    if(!getcwd(cwd, PATH_MAX)) {
        DEBUG_OUT("Path too long\n");
        abort();
    }
    fprintf(stdout, "%s\n", cwd);
    return 0;
}

int builtin_exit(char **argv) {
    exit(0);
}

#ifdef HOMEWORK
int builtin_favs(char **argv) {
    return -1;
}

static int recordatiorio_usage() {
    fprintf(stderr, "usage: set recordatorio <seconds> <message>\n");
    return 0;
}
int builtin_recordatorio(char **argv) {
    if(argv[1] == NULL || argv[2] == NULL || argv[3] == NULL || strcmp(argv[1], "recordatorio")) {
        recordatiorio_usage();
        return -1;
    }
    int after = atoi(argv[2]);
    if(after == 0) {
        recordatiorio_usage();
        return -1;
    }
    time_t t = time(NULL);
    long int t1 = (long int) t;
    DEBUG_OUT("Setting reminder at %ld\n", t1 + after);
    
    if(fork() == 0) {
        DEBUG_OUT("CHILD: Spawned, waiting...\n");
        int at = t1 + after;
        while((int) time(NULL) <= at) sleep(1);
        fprintf(stderr, "Reminder: %s\n", argv[3]);
    }
    return 0;
}


#endif

