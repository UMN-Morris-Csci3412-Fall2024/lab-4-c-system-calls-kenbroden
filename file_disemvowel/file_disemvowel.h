#ifndef FILE_DISEMVOWEL_H
#define FILE_DISEMVOWEL_H

#define UNIT_TESTING

#include <stdio.h>

// Function declarations
int is_vowel(char c);
void disemvowel(FILE *input, FILE *output);

// Main function declaration
#ifndef UNIT_TESTING
int main(int argc, char *argv[]);
#endif

#endif // FILE_DISEMVOWEL_H