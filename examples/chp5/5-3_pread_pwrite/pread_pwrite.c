#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>

#define OFFSET 10
#define BUFF_SIZE 400

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage %s src_file dst_file", argv[0]);
        exit(0);
    }
    
    //
    int fd1 = open(argv[1], O_RDONLY);
    char *buff = malloc(BUFF_SIZE);
    ssize_t numRead = pread(fd1, buff, BUFF_SIZE, OFFSET);

    //
    mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, filePerms);
    pwrite(fd2, buff, numRead, OFFSET);

    free(buff);
    close(fd1);
    close(fd2);

    return 0;
}