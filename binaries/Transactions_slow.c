#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

#define BEGIN_TRANSACTION _IO('S', 4)
#define END_TRANSACTION   _IO('S', 3)

int main(int argc, char **argv) {
    // Usage: ./Transactions_slow <file> <start_blk> <num_blks> <delay_sec> <tag>
    if (argc != 6) {
        fprintf(stderr, "Usage: %s <file> <start_blk> <num_blks> <delay_sec> <tag>\n", argv[0]);
        return -1;
    }

    char *filename = argv[1];
    int start_blk = atoi(argv[2]);
    int num_blks = atoi(argv[3]);
    int delay = atoi(argv[4]);
    char *tag = argv[5];

    int fd = open(filename, O_RDWR | O_CREAT, 0644);
    if (fd < 0) { perror("open"); return 1; }

    printf("[Writer] Starting transaction...\n");
    if (ioctl(fd, BEGIN_TRANSACTION) < 0) { perror("ioctl BEGIN"); return 1; }

    char buf[64];
    for (int i = 0; i < num_blks; i++) {
        int current_blk = start_blk + i;
        sprintf(buf, "[%s] Block_Update-%d\n", tag, current_blk);
        
        pwrite(fd, buf, strlen(buf), 4096 * current_blk);
        printf("[Writer] Wrote block %d. Sleeping %d seconds...\n", current_blk, delay);
        sleep(delay);
    }

    printf("[Writer] Committing transaction...\n");
    if (ioctl(fd, END_TRANSACTION) < 0) { perror("ioctl END"); return 1; }
    printf("[Writer] Transaction finished.\n");

    close(fd);
    return 0;
}
