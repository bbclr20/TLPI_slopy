#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<limits.h>
#include<errno.h>
#include"./get_num.h"

int errno;

static void
gnFail(const char *fname, const char *msg, const char *arg, const char *name) {
    fprintf(stderr, "%s error", fname);
    if (name != NULL)
        fprintf(stderr, " (in %s)", name);
    fprintf(stderr, ": %s\n", msg);
    if (arg != NULL && *arg != '\0')
        fprintf(stderr, "        offending text: %s\n", arg);
    exit(EXIT_FAILURE);
}

static long getNum(const char *fname, const char *arg, int flags, const char *name) {
    long res;
    char *endptr;
    int base;

    if (arg == NULL || *arg == '\0')
        gnFail(fname, "null or empty string", arg, name);

    base = (flags & GN_ANY_BASE) ? 0 : (flags & GN_BASE_8) ? 8 :
                        (flags & GN_BASE_16) ? 16 : 10;

    errno = 0;
    res = strtol(arg, &endptr, base);
    if (errno != 0)
        gnFail(fname, "strtol() failed", arg, name);

    if (*endptr != '\0')
        gnFail(fname, "nonnumeric characters", arg, name);

    if ((flags & GN_NONNEG) && res < 0)
        gnFail(fname, "negative value not allowed", arg, name);

    if ((flags & GN_GT_0) && res <= 0)
        gnFail(fname, "value must be > 0", arg, name);

    return res;
}

int getInt(const char *arg, int flags, const char *name) {
    long res;

    res = getNum("getInt", arg, flags, name);

    if (res > INT_MAX || res < INT_MIN)
        gnFail("getInt", "integer out of range", arg, name);

    return (int) res;
}

int main(int argc, char *argv[]) {
    pid_t childPid;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        printf("%s sleep-time...\n", argv[0]);
        return 0;
    }
    
    setbuf(stdout, NULL);
    
    for (int j = 1; j < argc; j++) {
        switch (fork()) {
            case -1:
                printf("Error forlk\n");
                exit(-1);
            case 0:
                printf(
                    "child %d started with PID %ld, sleeping %s seconds\n", 
                    j, 
                    (long) getpid(), 
                    argv[j]
                    ); 
                sleep(getInt(argv[j], GN_NONNEG, "sleep-time")); 
                exit(EXIT_SUCCESS);
            default:
                break;
        }
    }

    int numDead = 0;
    for(;;) {
        childPid = wait(NULL);
        if (childPid == -1) {
            if (errno == ECHILD) {
                    printf("No more children - bye!\n");
                    exit(EXIT_SUCCESS);
                } else {
                    printf("Error wait\n");
                    exit(-1);
            } 
        }
        numDead++;
        printf(
            "wait() returned child PID %ld (numDead=%d)\n",
            (long) childPid, 
            numDead
        );
    }
}