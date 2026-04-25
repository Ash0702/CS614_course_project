#include <stdio.h>
#include <stdlib.h>

void read_ascii_values(const char *filename, long offset, long num_bytes) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("Error opening file");
        return;
    }

    if (fseek(fp, offset, SEEK_SET) != 0) {
        perror("Error seeking file");
        fclose(fp);
        return;
    }

    unsigned char ch;
    long count = 0;

    printf("ASCII values from offset %ld:\n", offset);
    printf("----------------------------------\n");

    while ((num_bytes < 0 || count < num_bytes) &&
           fread(&ch, 1, 1, fp) == 1) {
        printf("%u ", ch);   // print ASCII / byte value
        count++;
    }

    printf("\n----------------------------------\n");
    printf("Read %ld bytes\n", count);

    fclose(fp);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <file> <offset> [num_bytes]\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    long offset = strtol(argv[2], NULL, 0);   // supports hex like 0x100
    long num_bytes = -1;

    if (argc >= 4)
        num_bytes = strtol(argv[3], NULL, 0);

    read_ascii_values(filename, offset, num_bytes);

    return 0;
}
