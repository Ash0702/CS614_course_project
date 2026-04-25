#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <num_lines>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    long num_lines = atol(argv[2]);
    if (num_lines <= 0) {
        fprintf(stderr, "Number of lines must be positive.\n");
        return 1;
    }

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("fopen");
        return 1;
    }

    const char *alphabet = "abcdefghijklmnopqrstuvwxyz";
    for (long i = 0; i < num_lines; i++) {
        fputs(alphabet, fp);
        fputc('\n', fp);
    }

    fclose(fp);
    printf("Generated file '%s' with %ld lines.\n", filename, num_lines);
    return 0;
}

