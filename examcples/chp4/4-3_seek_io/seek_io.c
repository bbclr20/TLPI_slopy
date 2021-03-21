#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include"get_num.h"

#define GN_ANY_BASE 0100


int main(int argc, char* argv[]) {
    size_t len;
    off_t offset;
    int fd, ap, j;
    char *buf;
    ssize_t numRead, numWritten;
    
    if (argc < 3 || strcmp(argv[1], "--help") == 0) {
        printf("%s file {r<length>|R<length>|w<string>|s<offset>}...\n", argv[0]);
    }

    //
    int fileFlags = O_RDWR | O_CREAT;
    mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    fd = open(argv[1], fileFlags, filePerms);
    if (fd == -1) {
        printf("Fail to open %s", argv[1]);
        return -1;
    }
    
    for (int ap = 2; ap < argc; ap++) {
        switch (argv[ap][0]) {
            /* Display bytes at current offset, as text */
            case 'r':
            /* Display bytes at current offset, in hex */
            case 'R':
                len = getLong(&argv[ap][1], GN_ANY_BASE, argv[ap]);
                buf = malloc(len);
                if (buf == NULL) {
                    printf("Error malloc\n");
                }

                numRead = read(fd, buf, len);
                if (numRead == -1){
                    printf("Error read\n");
                }

                if (numRead == 0) {
                    printf("%s: end-of-file\n", argv[ap]);
                } else {
                    printf("%s: ", argv[ap]);
                    for (j = 0; j < numRead; j++) {
                        if (argv[ap][0] == 'r') {
                            // printf("%c", isprint((unsigned char) buf[j]) ? buf[j] : '?');
                            printf("%c", buf[j]);
                        } else {
                            printf("%02x ", (unsigned int) buf[j]);
                        }
                    }
                    printf("\n");
                }
                free(buf);
                break;

            /* Write string at current offset */
            case 'w':
                numWritten = write(fd, &argv[ap][1], strlen(&argv[ap][1]));
                if (numWritten == -1) {
                    printf("Error write\n");
                }
                printf("%s: wrote %ld bytes\n", argv[ap], (long) numWritten);
                break;

            /* Change file offset */
            case 's':
                offset = getLong(&argv[ap][1], GN_ANY_BASE, argv[ap]);
                // use SEEK_SET to seek from the start of the file
                if (lseek(fd, offset, SEEK_CUR) == -1) {
                    printf("Error lseek\n");
                }
                printf("%s: seek succeeded\n", argv[ap]);
                break;

            default:
                printf("Argument must start with [rRws]: %s\n", argv[ap]);
        }
    }

}