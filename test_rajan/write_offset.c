#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char *argv[]) {
    if (argc != 4) {
        dprintf(2, "Usage: %s <filename> <offset> <string>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    off_t offset = atoll(argv[2]);
    const char *text = argv[3];

    int fd = open(filename, O_WRONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // Move file pointer to the given offset
    if (lseek(fd, offset, SEEK_SET) == (off_t)-1) {
        perror("lseek");
        close(fd);
        return 1;
    }

    ssize_t len = write(fd, text, strlen(text));
    if (len < 0) {
        perror("write");
        close(fd);
        return 1;
    }

    dprintf(1, "✅ Wrote \"%s\" (%zd bytes) at offset %lld in file '%s'.\n",
            text, len, (long long)offset, filename);

    close(fd);
    return 0;
}

