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

// Returns time difference in seconds
double get_time_diff(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

int main(int argc, char **argv) {
    // Usage: ./bench_write <file> <size_MB> <use_transaction_flag: 0|1>
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <file> <size_MB> <use_txn: 0|1>\n", argv[0]);
        return -1;
    }

    char *filename = argv[1];
    long long size_mb = atoi(argv[2]);
    int use_txn = atoi(argv[3]);
    long long num_blocks = (size_mb * 1024 * 1024) / BLOCK_SIZE;

    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) { perror("open"); return 1; }

    char *buf = aligned_alloc(4096, BLOCK_SIZE);
    memset(buf, 'A', BLOCK_SIZE); // Fill with dummy data

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    if (use_txn && ioctl(fd, BEGIN_TRANSACTION) < 0) {
        perror("ioctl BEGIN"); return 1;
    }

    for (long long i = 0; i < num_blocks; i++) {
        if (write(fd, buf, BLOCK_SIZE) != BLOCK_SIZE) {
            perror("write"); return 1;
        }
    }

    if (use_txn && ioctl(fd, END_TRANSACTION) < 0) {
        perror("ioctl END"); return 1;
    }

    // CRITICAL: Force the kernel to flush the page cache to physical disk
    fsync(fd);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = get_time_diff(start, end);
    double throughput = size_mb / elapsed;

    // Output strictly the CSV format: Size_MB,Time_s,Throughput_MBps
    printf("%lld,%.4f,%.2f\n", size_mb, elapsed, throughput);

    free(buf);
    close(fd);
    return 0;
}
