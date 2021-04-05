#include<signal.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<limits.h>
#include<string.h>

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
    
    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        printf("Usuage: %s sig-num pid\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    int s, sig;
    pid_t pid;
    sig = getInt(argv[2], 0, "sig-num");
    pid = getInt(argv[1], 0, "pid");
    printf("sig: %d pid: %d \n", sig, pid);
    s = kill(pid, sig);
    
    if (sig != 0) {
        if (s == -1) {
            printf("Fail to kill the process: %d\n", pid);
            exit(EXIT_FAILURE);
        }
    } else {
        if (s == 0) {
            printf("Process exists and we can send it a signal\n");
        } else {
            if (errno == EPERM) {
                printf("Process exists, but we don't have permission to send it a signal\n");
            } else if (errno == ESRCH) {
                printf("Process does not exist\n");
            } else {
                printf("Error kill\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    exit(EXIT_SUCCESS);
}
