#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>

// =====================
// 🔧 DEFINE THESE YOURSELF
// =====================
#define BEGIN_TRANSACTION _IO('S', 4)   // <-- replace if needed
#define END_TRANSACTION   _IO('S', 3)   // <-- replace if needed

// If your ioctl needs args, define structs here
// struct txn_args {
//     ...
// };

#define FILE_PATH "testfile.bin"

int main(int argc, char **argv)
{
    int fd;
    ssize_t ret;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file> <write_len_num_blks>\n", argv[0]);
        return -1;
    }

    int write_len = atoi(argv[2]);
    char *filename = argv[1];

    // Open file
    fd = open(filename, O_RDWR | O_CREAT, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    printf("File opened successfully.\n");

    // =====================
    // 🚀 BEGIN TRANSACTION
    // =====================
    if (ioctl(fd, BEGIN_TRANSACTION) < 0) {
        perror("ioctl BEGIN_TRANSACTION");
        close(fd);
        return 1;
    }

    printf("Transaction started.\n");

    char buf[32];

    for (int i = 0; i < write_len; i++) {
        sprintf(buf, "TransactionedBlock-%d\n", i + 1);

        ret = pwrite(fd, buf, strlen(buf), 4096 * i);
        if (ret < 0) {
            perror("pwrite");
        }
    }

    printf("Writes completed.\n");

    if (ioctl(fd, END_TRANSACTION) < 0) {
        perror("ioctl END_TRANSACTION");
        close(fd);
        return 1;
    }

    printf("Transaction ended.\n");

    close(fd);
    return 0;
}
