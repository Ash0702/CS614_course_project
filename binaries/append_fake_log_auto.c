#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#pragma pack(push, 1)
struct scorw_log_record {
    uint32_t version_num;
    uint32_t logical_start_blk;
    uint64_t physical_start_blk;
    uint32_t len_blks;
    uint32_t padding;
    uint64_t padding2;
};
#pragma pack(pop)

int main(int argc, char *argv[]) {
    // Usage: ./append_fake_log_auto <logfile> <version> <logical> <physical> <length>
    if (argc != 6) {
        printf("Usage: %s <logfile> <version> <logical> <physical> <length>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    FILE *fp = fopen(filename, "ab");
    if (!fp) {
        perror("fopen");
        return 1;
    }

    struct scorw_log_record rec;
    rec.version_num = atoi(argv[2]);
    rec.logical_start_blk = atoi(argv[3]);
    rec.physical_start_blk = strtoull(argv[4], NULL, 10);
    rec.len_blks = atoi(argv[5]);
    rec.padding = 0;
    rec.padding2 = 0;

    if (fwrite(&rec, sizeof(rec), 1, fp) != 1) {
        perror("fwrite");
        fclose(fp);
        return 1;
    }

    fclose(fp);
    printf("  [+] Appended fake record: V=%u L=%u P=%llu Len=%u\n", 
           rec.version_num, rec.logical_start_blk, 
           (unsigned long long)rec.physical_start_blk, rec.len_blks);
    return 0;
}
