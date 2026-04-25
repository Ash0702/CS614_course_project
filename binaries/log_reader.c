#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>

// Using your exact 32-byte layout, translated to user-space types
#pragma pack(push, 1)
struct scorw_log_record {
    uint32_t version_num;       // e.g., Version 2
    uint32_t logical_start_blk; // Where the user wrote it (e.g., Block 5)
    uint64_t physical_start_blk;// Where Ext4 actually put it on disk
    uint32_t len_blks;          // How many blocks were written contiguously
    uint32_t padding;           // Ensures 64-bit memory alignment
    uint64_t padding2;          // Ensures 32 bytes, which can divide 4Kb
};
#pragma pack(pop)

int main(int argc, char **argv) {
    if (argc != 4) {
        printf("Usage: %s <log_file> <start_record_index> <num_records>\n", argv[0]);
        printf("Note: <start_record_index> is 0-indexed. 0 is the first record.\n");
        return 1;
    }

    const char *filename = argv[1];
    long long start_record = atoll(argv[2]);
    int num_records = atoi(argv[3]);

    size_t record_size = sizeof(struct scorw_log_record);
    
    // Calculate the exact byte offset (Index * 32 bytes)
    long long start_offset = start_record * record_size;

    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("❌ Error opening log file");
        return 1;
    }

    if (lseek(fd, start_offset, SEEK_SET) < 0) {
        perror("❌ Error seeking to offset");
        close(fd);
        return 1;
    }

    struct scorw_log_record rec;

    printf("📂 Reading %d records starting at index %lld (offset %lld bytes)...\n", 
           num_records, start_record, start_offset);
    printf("--------------------------------------------------------------------------------\n");
    printf("%-8s | %-8s | %-15s | %-15s | %-10s\n", "Index", "Version", "Logical Blk", "Physical Blk", "Length");
    printf("--------------------------------------------------------------------------------\n");

    for (int i = 0; i < num_records; i++) {
        ssize_t bytes_read = read(fd, &rec, record_size);
        
        if (bytes_read == 0) {
            printf("⚠️  EOF reached. No more records.\n");
            break;
        } else if (bytes_read < record_size) {
            printf("❌ Partial read error at index %lld (read %zd of %zu bytes).\n", start_record + i, bytes_read, record_size);
            break;
        }

        printf("%-8lld | %-8u | %-15u | %-15llu | %-10u\n", 
               start_record + i, 
               rec.version_num, 
               rec.logical_start_blk, 
               (unsigned long long)rec.physical_start_blk,
               rec.len_blks);
    }

    printf("--------------------------------------------------------------------------------\n");
    close(fd);
    return 0;
}
