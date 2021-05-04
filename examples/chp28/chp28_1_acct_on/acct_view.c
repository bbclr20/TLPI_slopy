#include<fcntl.h>
#include<time.h>
#include<sys/stat.h>
#include<sys/acct.h>
#include<limits.h>
#include<sys/types.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include <pwd.h>
#include <grp.h>

#define TIME_BUF_SIZE 100

// #include "ugid_functions.h"
// #include "tlpi_hdr.h"

static long long comptToLL(comp_t ct) {
    const int EXP_SIZE = 3;
    const int MANTISSA_SIZE = 13; /* Followed by 13-bit mantissa */ const int MANTISSA_MASK = (1 << MANTISSA_SIZE) - 1;
    long long mantissa, exp;
    mantissa = ct & MANTISSA_MASK;
    exp = (ct >> MANTISSA_SIZE) & ((1 << EXP_SIZE) - 1);
    return mantissa << (exp * 3);
}
char* userNameFromId(uid_t uid) {
    struct passwd *pwd;

    pwd = getpwuid(uid);
    return (pwd == NULL) ? NULL : pwd->pw_name;
}

int main(int argc, char *argv[]) {
    int acctFile;
    struct acct ac;
    ssize_t numRead;
    char *s;
    char timeBuf[TIME_BUF_SIZE]; struct tm *loc;
    time_t t;

    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        printf("%s file\n", argv[0]);
        return -1;
    }

    acctFile = open(argv[1], O_RDONLY); 
    if (acctFile == -1) {
        printf("Error open acctFile\n");
        return -1;
    }

    printf("command  flags term. user start time        CPU   elapsed\n");
    printf("               status                       time  time\n");
    while ((numRead = read(acctFile, &ac, sizeof(struct acct))) > 0) { 
        if (numRead != sizeof(struct acct)) {
            printf("Error partial read\n");
            return -1;
        }

        printf("%-8.8s  ", ac.ac_comm);
        printf("%c", (ac.ac_flag & AFORK) ? 'F' : '-') ;
        printf("%c", (ac.ac_flag & ASU) ? 'S' : '-') ;
        printf("%c", (ac.ac_flag & AXSIG) ? 'X' : '-') ;
        printf("%c", (ac.ac_flag & ACORE) ? 'C' : '-') ;

        // #ifdef __linux__
        //     printf(" %#6lx ", (unsigned long) ac.ac_exitcode);
        // #else
        //     printf(" %#6lx ", (unsigned long) ac.ac_stat);
        // #endif

        s = userNameFromId(ac.ac_uid); 
        printf("%-8.8s ", (s == NULL) ? "???" : s);

        t = ac.ac_btime;
        loc = localtime(&t);
        if (loc == NULL) {
            printf("???Unknown time???  ");
        } else {
            strftime(timeBuf, TIME_BUF_SIZE, "%Y-%m-%d %T ", loc);
            printf("%s ", timeBuf);
        }
        printf("%5.2f %7.2f ", 
                (double) (comptToLL(ac.ac_utime) + comptToLL(ac.ac_stime)) / sysconf(_SC_CLK_TCK), 
                (double) comptToLL(ac.ac_etime) / sysconf(_SC_CLK_TCK)
                ); 
        printf("\n");
    }
    close(acctFile);
    return 0;
}