#include <stdio.h>
#include <stdbool.h>
#include "file_disemvowel.h"
#include <stdlib.h>

#define BUF_SIZE 1024

bool is_vowel(char c) {
    /*
     * Returns true if c is a vowel (upper or lower case), and
     * false otherwise.
     */
    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
        c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
        return true;
    } else {
        return false;
    }
}

int copy_non_vowels(int num_chars, char* in_buf, char* out_buf) {
    /*
     * Copy all the non-vowels from in_buf to out_buf.
     * num_chars indicates how many characters are in in_buf,
     * and this function should return the number of non-vowels that
     * that were copied over.
     */
    int non_vowels = 0;
    for (int i = 0; i < num_chars; i++) {
        if (!is_vowel(in_buf[i])) {
            out_buf[non_vowels] = in_buf[i];
            non_vowels++;
        }
    }
    return non_vowels;
}

void disemvowel(FILE* inputFile, FILE* outputFile) {
    /*
     * Copy all the non-vowels from inputFile to outputFile.
     * Create input and output buffers, and use fread() to repeatedly read
     * in a buffer of data, copy the non-vowels to the output buffer, and
     * use fwrite to write that out.
     */
    char in_buf[BUF_SIZE];
    char out_buf[BUF_SIZE];
    int num_chars;

    while ((num_chars = fread(in_buf, sizeof(char), BUF_SIZE, inputFile)) > 0) {
        int num_non_vowels = copy_non_vowels(num_chars, in_buf, out_buf);
        fwrite(out_buf, sizeof(char), num_non_vowels, outputFile);
    }

    // free(in_buf);
    // free(out_buf);

    // return 0;
}

int main(int argc, char *argv[]) {
    // This sets these to `stdin` and `stdout` by default.
    // You then need to set them to user specified files when the user
    // provides files names as command line arguments.
    FILE *inputFile = stdin;
    FILE *outputFile = stdout;

    if (argc == 2) {
        inputFile = fopen(argv[1], "r");
        if (inputFile == NULL) {
            perror("Failed to open input file");
            return 1;
        }
    } else if (argc == 3) {
        inputFile = fopen(argv[1], "r");
        if (inputFile == NULL) {
            perror("Failed to open input file");
            return 1;
        }
        outputFile = fopen(argv[2], "w");
        if (outputFile == NULL) {
            perror("Failed to open output file");
            fclose(inputFile);
            return 1;
        }
    }

    disemvowel(inputFile, outputFile);

    if (argc >= 2) {
        fclose(inputFile);
    }
    if (argc == 3) {
        fclose(outputFile);
    }

    return 0;
}