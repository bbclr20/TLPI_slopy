#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>

#define OFFSET 10
#define BUFF_SIZE 300

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s src_filei\n", argv[0]);
        exit(-1);
    }

    // open fd1 and change the offset
    int fd1 = open(argv[1], O_RDONLY);
    if (fd1 == -1) {
        printf("Fail to open %s\n", argv[1]);
        exit(-1);
    }
    lseek(fd1, OFFSET, SEEK_SET);

    // dup fd2 based on fd1
    int fd2 = dup(fd1);
    char *buff = malloc(BUFF_SIZE);
    int numRead = read(fd2, buff, BUFF_SIZE);
    for (int j=0; j<numRead; j++) {
        printf("%c", buff[j]);
    }
    printf("\n");

    // close all
    free(buff);
    close(fd1);
    close(fd2);

    return 0;
}