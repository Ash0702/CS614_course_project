#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        dprintf(2, "Usage: %s <filename> <offset> <char>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    off_t offset = atoll(argv[2]);
    char ch = argv[3][0];

    // Open file (create if not exists)
    int fd = open(filename, O_WRONLY | O_CREAT, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // Move to specified offset
    if (lseek(fd, offset, SEEK_SET) == (off_t)-1) {
        perror("lseek");
        close(fd);
        return 1;
    }

    // Prepare 4096-byte buffer filled with the specified character
    char buf[4096];
    memset(buf, ch, sizeof(buf));

    // Write buffer to file
    ssize_t written = write(fd, buf, sizeof(buf));
    if (written < 0) {
        perror("write");
        close(fd);
        return 1;
    }

    dprintf(1, "✅ Wrote %zd bytes of '%c' starting from offset %lld in '%s'.\n",
            written, ch, (long long)offset, filename);

    close(fd);
    return 0;
}

