#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

// 1GB = 1024 * 1024 * 1024 bytes
#define ONE_GB (1024LL * 1024LL * 1024LL)
#define BUFFER_SIZE (1024 * 1024) // 1MB buffer for efficient writing

int main(int argc, char **argv)
{
    int fd;
    char *buffer;
    ssize_t bytes_written;
    long long total_written = 0;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return -1;
    }

    char *filename = argv[1];

    // O_CREAT: Create if doesn't exist
    // O_RDWR: Read/Write mode
    // O_TRUNC: Clear existing content to ensure it starts fresh
    fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror("Error opening file");
        return 1;
    }

    // Allocate 1MB of memory and fill it with dummy data (e.g., 'A')
    buffer = malloc(BUFFER_SIZE);
    if (!buffer)
    {
        perror("Failed to allocate buffer");
        close(fd);
        return 1;
    }
    memset(buffer, 'A', BUFFER_SIZE);

    printf("Preparing file '%s'...\n", filename);
    printf("Overwriting with 1GB of data. Please wait...\n");

    while (total_written < ONE_GB)
    {
        bytes_written = write(fd, buffer, BUFFER_SIZE);

        if (bytes_written < 0)
        {
            perror("Write error");
            free(buffer);
            close(fd);
            return 1;
        }

        total_written += bytes_written;

        // Optional: Print progress every 100MB
        if ((total_written % (100 * 1024 * 1024)) == 0)
        {
            printf("Progress: %lld MB / 1024 MB\n", total_written / (1024 * 1024));
        }
    }

    // Ensure data is flushed to disk
    fsync(fd);

    printf("Successfully created/overwrote '%s' with 1GB of data.\n", filename);

    free(buffer);
    close(fd);
    return 0;
}
