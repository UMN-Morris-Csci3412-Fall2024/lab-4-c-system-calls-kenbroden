#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Checks to see if a character is a vowel
int is_vowel(char c) {
    char lower = tolower(c);
    return lower == 'a' || lower == 'e' || lower == 'i' || lower == 'o' || lower == 'u';
}

// Reads from input and writes to output, removing all vowels
void disemvowel(FILE *input, FILE *output) {
    char c;
    while ((c = fgetc(input)) != EOF) {
        if (!is_vowel(c)) {
            fputc(c, output);
        }
    }
}

// Main function that reads from input and writes to output
int main(int argc, char *argv[]) {
    FILE *input = stdin;  // Default to stdin
    FILE *output = stdout;  // Default to stdout

    if (argc > 1) {
        input = fopen(argv[1], "r");
        if (input == NULL) {
            perror("Error opening input file");
            return EXIT_FAILURE;
        }
    }

    if (argc > 2) {
        output = fopen(argv[2], "w");
        if (output == NULL) {
            perror("Error opening output file");
            if (input != stdin) fclose(input);
            return EXIT_FAILURE;
        }
    }

    disemvowel(input, output);

    if (input != stdin) fclose(input);
    if (output != stdout) fclose(output);

    return EXIT_SUCCESS;
}