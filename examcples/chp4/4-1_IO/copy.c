#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

#define BUFF_SIZE 1024

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usuage: %s src_file dst_file\n", argv[0]);
    }

    // open read file
    int inputFd = open(argv[1], O_RDONLY);
    if (inputFd == -1) {
        printf("Fail to open %s", argv[1]);
    }

    // open write file
    int openFlags = O_CREAT | O_WRONLY;
    mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    printf("filePerms: %d\n", filePerms);

    int outputFd = open(argv[2], openFlags, filePerms);
    if (outputFd == -1) {
        printf("Fail to open %s", argv[2]);
    }

    // read write
    ssize_t numRead = 0;
    char buf[BUFF_SIZE];
    while ((numRead = read(inputFd, buf, BUFF_SIZE)) > 0) {
        if (write(outputFd, buf, numRead) != numRead) {
            printf("Couldn't write whole buff\n");
        }
    }
    printf("Copy done\n");

    // close all files
    if (close(inputFd) == -1) {
        printf("Fail to close %s", argv[1]);
    }

    if (close(outputFd) == -1) {
        printf("Fail to close %s", argv[2]);
    }
    return 0;
}