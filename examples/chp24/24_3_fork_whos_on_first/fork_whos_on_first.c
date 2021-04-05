#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<unistd.h>

#define GN_NONNEG       01      /* Value must be >= 0 */
#define GN_GT_0         02      /* Value must be > 0 */
                                /* By default, integers are decimal */
#define GN_ANY_BASE   0100      /* Can use any base - like strtol(3) */
#define GN_BASE_8     0200      /* Value is expressed in octal */
#define GN_BASE_16    0400      /* Value is expressed in hexadecimal */

static void
gnFail(
    const char *fname, 
    const char *msg, 
    const char *arg, 
    const char *name
    )
{
    fprintf(stderr, "%s error", fname);
    if (name != NULL)
        fprintf(stderr, " (in %s)", name);
    fprintf(stderr, ": %s\n", msg);
    if (arg != NULL && *arg != '\0')
        fprintf(stderr, "        offending text: %s\n", arg);

    exit(EXIT_FAILURE);
}

static long
getNum(
    const char *fname, 
    const char *arg, 
    int flags, 
    const char *name
    )
{
    long res;
    char *endptr;
    int base;

    if (arg == NULL || *arg == '\0')
        gnFail(fname, "null or empty string", arg, name);

    base = (flags & GN_ANY_BASE) ? 0 : (flags & GN_BASE_8) ? 8 :
                        (flags & GN_BASE_16) ? 16 : 10;

    int errno = 0;
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
    int numChildren, j;
    pid_t childPid;
    
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        printf("Usuage: %s [num-children]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    numChildren = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-children") : 1;
    setbuf(stdout, NULL);
    
    /* Make stdout unbuffered */
    for (j = 0; j < numChildren; j++) {
        switch (childPid = fork()) {
        case -1:
            printf("Error during fork process\n");
            exit(EXIT_FAILURE);
        case 0:
            printf("%d child\n", j);
            exit(EXIT_SUCCESS);
        default:
            printf("%d parent\n", j);
            wait(NULL);
            break;
        }
    }

    /* Wait for child to terminate */
    exit(EXIT_SUCCESS);
}
