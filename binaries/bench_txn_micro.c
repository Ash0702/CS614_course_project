#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <time.h>

#define BEGIN_TRANSACTION _IO('S', 4)
#define END_TRANSACTION   _IO('S', 3)
#define BLOCK_SIZE 4096

long long get_time_us(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) * 1000000LL + (end.tv_nsec - start.tv_nsec) / 1000LL;
}

int main(int argc, char **argv) {
    // Usage: ./bench_txn_micro <file> <size_MB> <use_txn: 0|1>
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <file> <size_MB> <use_txn>\n", argv[0]);
        return -1;
    }

    char *filename = argv[1];
    long long size_mb = atoi(argv[2]);
    int use_txn = atoi(argv[3]);
    long long num_blocks = (size_mb * 1024 * 1024) / BLOCK_SIZE;

    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) { perror("open"); return 1; }

    char *buf = aligned_alloc(4096, BLOCK_SIZE);
    memset(buf, 'X', BLOCK_SIZE);

    struct timespec t0, t1, t2, t3, t4;
    long long begin_us = 0, write_us = 0, end_us = 0, fsync_us = 0;

    // Phase 1: Begin Transaction
    clock_gettime(CLOCK_MONOTONIC, &t0);
    if (use_txn) {
        if (ioctl(fd, BEGIN_TRANSACTION) < 0) { perror("ioctl BEGIN"); return 1; }
    }
    clock_gettime(CLOCK_MONOTONIC, &t1);

    // Phase 2: Writes
    for (long long i = 0; i < num_blocks; i++) {
        if (write(fd, buf, BLOCK_SIZE) != BLOCK_SIZE) { perror("write"); return 1; }
    }
    clock_gettime(CLOCK_MONOTONIC, &t2);

    // Phase 3: End Transaction
    if (use_txn) {
        if (ioctl(fd, END_TRANSACTION) < 0) { perror("ioctl END"); return 1; }
    }
    clock_gettime(CLOCK_MONOTONIC, &t3);

    // Phase 4: Fsync
    fsync(fd);
    clock_gettime(CLOCK_MONOTONIC, &t4);

    // Calculate timings
    if (use_txn) begin_us = get_time_us(t0, t1);
    write_us = get_time_us(t1, t2);
    if (use_txn) end_us = get_time_us(t2, t3);
    fsync_us = get_time_us(t3, t4);

    // Print CSV format with strictly the data
    printf("%lld,%lld,%lld,%lld,%lld\n", size_mb, begin_us, write_us, end_us, fsync_us);

    free(buf);
    close(fd);
    return 0;
}
