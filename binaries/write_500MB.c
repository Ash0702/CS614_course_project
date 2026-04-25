#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define FILE_SIZE (500UL * 1024 * 1024)   // 500 MB
#define CHUNK_SIZE (1UL * 1024 * 1024)    // 1 MB per write

int main(void) {
    int fd;
    char *buffer;
    size_t total_written = 0;

    fd = open("output.bin", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    buffer = malloc(CHUNK_SIZE);
    if (!buffer) {
        perror("malloc");
        close(fd);
        return 1;
    }

    /* Fill buffer with some pattern */
    memset(buffer, 'A', CHUNK_SIZE);

    while (total_written < FILE_SIZE) {
        size_t to_write = CHUNK_SIZE;
        if (FILE_SIZE - total_written < CHUNK_SIZE)
            to_write = FILE_SIZE - total_written;

        ssize_t n = write(fd, buffer, to_write);
        if (n < 0) {
            perror("write");
            free(buffer);
            close(fd);
            return 1;
        }

        total_written += n;
    }

    printf("Successfully wrote %zu bytes (500 MB)\n", total_written);

    free(buffer);
    close(fd);
    return 0;
}
