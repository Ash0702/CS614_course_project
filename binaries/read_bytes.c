#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

/*
 * Usage:
 *   ./readbytes <filename> <num_bytes>
 *
 * Example:
 *   ./readbytes test.txt 4096
 */

int main(int argc, char *argv[])
{
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <filename> <num_bytes> <offset>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    long n = atol(argv[2]);
    long offset = atol(argv[3]);
    if (n <= 0) {
        fprintf(stderr, "Invalid byte count\n");
        return 1;
    }

    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    char *buf = malloc(n);
    if (!buf) {
        perror("malloc");
        close(fd);
        return 1;
    }
    lseek(fd , offset , SEEK_SET);
    ssize_t bytes_read = read(fd, buf, n);
    if (bytes_read < 0) {
        perror("read");
        free(buf);
        close(fd);
        return 1;
    }

    printf("Requested: %ld bytes\n", n);
    printf("Read: %zd bytes\n", bytes_read);

    /* Optional: print contents */
    fwrite(buf, 1, bytes_read, stdout);
    printf("\n");

    free(buf);
    close(fd);
    return 0;
}
