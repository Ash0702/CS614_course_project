#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct scorw_log_record {
    uint32_t version_num;
    uint32_t logical_start_blk;
    uint64_t physical_start_blk;
    uint32_t len_blks;
    uint32_t padding;
    uint64_t padding2;
};

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s <logfile>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];

    FILE *fp = fopen(filename, "ab");
    if (!fp) {
        perror("fopen");
        return 1;
    }

    int n;
    printf("Enter number of records to add: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        struct scorw_log_record rec;

        printf("\nRecord %d\n", i + 1);
        printf("Enter: <version> <logical> <physical> <length>\n");

        scanf("%u %u %llu %u",
              &rec.version_num,
              &rec.logical_start_blk,
              (unsigned long long *)&rec.physical_start_blk,
              &rec.len_blks);

        rec.padding = 0;
        rec.padding2 = 0;

        if (fwrite(&rec, sizeof(rec), 1, fp) != 1) {
            perror("fwrite");
            fclose(fp);
            return 1;
        }

        printf("Added record %d successfully.\n", i + 1);
    }

    fclose(fp);

    printf("\nAll records appended to %s\n", filename);
    printf("Record size = %zu bytes\n", sizeof(struct scorw_log_record));

    return 0;
}
