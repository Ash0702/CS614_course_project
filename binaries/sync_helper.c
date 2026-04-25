#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }

    fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    if (fsync(fd) < 0) {
        perror("fsync");
        close(fd);
        return 1;
    }

    printf("fsync successful for %s\n", argv[1]);

    close(fd);
    return 0;
}
