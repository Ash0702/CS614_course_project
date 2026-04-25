#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main(int argc , char **argv) {
    int fd;
    if(argc != 2){
	printf("Usage : <%s> <file_name>" , argv[0]);
    }
    const char *filename = argv[1];
    const char *data = "Hello, fsync world!\n";

    // Open file (create if not exists)
    fd = open(filename, O_RDWR, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // Write data
    ssize_t written = write(fd, data, strlen(data));
    if (written < 0) {
        perror("write");
        close(fd);
        return 1;
    }

    printf("Wrote %ld bytes\n", written);

    // Force data + metadata to disk
    if (fsync(fd) < 0) {
        perror("fsync");
        close(fd);
        return 1;
    }

    printf("fsync completed successfully\n");

    close(fd);
    return 0;
}
