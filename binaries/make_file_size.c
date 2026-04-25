#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 4096

int main(int argc , char ** argv) {
    int fd;
    char buffer[64]; // Increased buffer size to hold the text + numbers

    if(argc != 3){
        printf("Usage : %s <filename> <block>\n" , argv[0]);
        return 0;
    }
    char *file_name = argv[1];
    int blocks = atoi(argv[2]);

    // Open or create the file
    fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        exit(1);
    }

    for (int i = 0; i < blocks; i++) {
        // Format the string dynamically with the current block number (i)
        snprintf(buffer, sizeof(buffer), "Init_%d\n", 1+i);
        
        // Get the exact length of the new string so we write exactly that many bytes
        int len = strlen(buffer);

        // Jump to the start of the next 4KB block
        lseek(fd, i * BLOCK_SIZE, SEEK_SET);
        
        ssize_t written = write(fd, buffer, len);
        if (written < 0) {
            perror("write");
            close(fd);
            exit(1);
        }
    }

    close(fd);
    printf("File created: %s with %d blocks\n", file_name, blocks);
    return 0;
}
