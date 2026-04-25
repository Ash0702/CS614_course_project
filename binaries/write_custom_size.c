#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define CHUNK_SIZE (1UL * 1024 * 1024)    // 1 MB per write

int main(int argc, char *argv[]) {
    int fd;
    char *buffer;
    size_t total_written = 0;
    size_t file_size;
    unsigned long size_in_mb;
    const char *filename;

    /* Check for the correct number of arguments */
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <size_in_MB>\n", argv[0]);
        return 1;
    }

    filename = argv[1];

    /* Parse the command line argument for size */
    errno = 0;
    size_in_mb = strtoul(argv[2], NULL, 10);
    if (errno != 0) {
        perror("Invalid size specified");
        return 1;
    }

    file_size = size_in_mb * 1024 * 1024; // Convert MB to Bytes

    /* * Open existing file. 
     * O_WRONLY: Write-only mode.
     * O_CREAT: Still included as a fallback to create the file if it somehow doesn't exist.
     * Notice O_TRUNC is removed. Data will overwrite from the beginning (offset 0).
     */
    fd = open(filename, O_WRONLY | O_CREAT, 0644);
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

    /* Fill buffer with a pattern ('B' to distinguish from previous 'A's) */
    memset(buffer, 'B', CHUNK_SIZE);

    while (total_written < file_size) {
        size_t to_write = CHUNK_SIZE;
        if (file_size - total_written < CHUNK_SIZE)
            to_write = file_size - total_written;

        ssize_t n = write(fd, buffer, to_write);
        if (n < 0) {
            perror("write");
            free(buffer);
            close(fd);
            return 1;
        }

        total_written += n;
    }

    printf("Successfully wrote %zu bytes (%lu MB) to '%s'\n", total_written, size_in_mb, filename);

    free(buffer);
    close(fd);
    return 0;
}
