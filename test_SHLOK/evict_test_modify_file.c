#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>

// Ioctl definitions
#define BEGIN_TRANSACTION _IO('S', 4)
#define END_TRANSACTION _IO('S', 3)

#define BLOCK_SIZE 4096
#define BLOCKS_PER_WRITE 50
#define TOTAL_BLOCKS 200
#define BUFFER_SIZE (BLOCK_SIZE * BLOCKS_PER_WRITE)

int main(int argc, char **argv)
{
    int fd;
    // Allocate a buffer large enough for 50 blocks
    char *buffer = malloc(BUFFER_SIZE);
    if (!buffer)
    {
        perror("Failed to allocate buffer");
        return 1;
    }

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        free(buffer);
        return -1;
    }

    fd = open(argv[1], O_RDWR);
    if (fd < 0)
    {
        perror("Error opening file");
        free(buffer);
        return 1;
    }

    printf("Starting test: Writing 200 blocks using 4 writes of 50 blocks each...\n");

    // Start the transaction for the entire operation
    if (ioctl(fd, BEGIN_TRANSACTION) < 0)
    {
        perror("Error: BEGIN_TRANSACTION");
        close(fd);
        free(buffer);
        return 1;
    }

    // Loop 4 times (4 * 50 = 200 blocks total)
    for (int i = 0; i < 4; i++)
    {
        char fill_char = 'A' + i; // 'A', 'B', 'C', 'D'
        memset(buffer, fill_char, BUFFER_SIZE);

        off_t offset = (off_t)i * BUFFER_SIZE;
        printf("Write %d: Writing 50 blocks at offset %lld (Char: %c)...\n", i + 1, (long long)offset, fill_char);

        if (pwrite(fd, buffer, BUFFER_SIZE, offset) < 0)
        {
            fprintf(stderr, "Error in pwrite at iteration %d: %s\n", i + 1, strerror(errno));
            break;
        }
    }

    // End (Commit) the transaction
    if (ioctl(fd, END_TRANSACTION) < 0)
    {
        perror("Error: END_TRANSACTION");
    }
    else
    {
        printf("Transaction committed successfully for all 200 blocks.\n");
    }

    free(buffer);
    close(fd);
    return 0;
}
