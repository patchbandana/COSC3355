/*  Pat Eizenga
 *  COSC3355.001
 *  2/12/2025
 *  Purpose: A wordcount program that takes a file path and
 *   outputs the number of lines, words, and bytes for each
 *   file using limited libraries and POSIX API calls.
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

// Hard coded limit for Linux command line buffer
#define BUFFER_SIZE 4096

/* Function prototype for processing a file */
void process_file(const char *filename, int *totalLines, int *totalWords, int *totalBytes);

int main(int argc, char *argv[]) {
    /* Check if the program has at least one file argument */
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> [file2 ...]\n", argv[0]);
        return 1;
    }

    /* Initialize counters for total counts across multiple files */
    int totalLines = 0;
    int totalWords = 0;
    int totalBytes = 0;

    /* Iterate through each file passed as a command-line argument */
    for (int i = 1; i < argc; i++) {
        process_file(argv[i], &totalLines, &totalWords, &totalBytes);
    }

    /* If more than one file was processed, print a cumulative total */
    if (argc > 2) {
        printf("%d %d %d total\n", totalLines, totalWords, totalBytes);
    }
    return 0;
}

void process_file(const char *filename, int *totalLines, int *totalWords, int *totalBytes) {
    /* Attempt to open the file for reading */
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Error 404: file %s not found!\n", filename);
        return;
    }

    char buffer[BUFFER_SIZE]; // Buffer to hold file data during read
    ssize_t bytes_read; // Stores number of bytes read from the file
    int lines = 0; // Line counter
    int words = 0; // Word counter
    int bytes = 0; // Byte counter
    int inWord = 0; // Tracks if we are inside a word

    /* Read the file in chunks until the end */
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        bytes += bytes_read; // Accumulate total byte count
        for (ssize_t i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n') {
                lines++; // Increment line count on newline character
            }
            if (buffer[i] == ' ' || buffer[i] == '\t' || buffer[i] == '\n') {
                if (inWord) {
                    words++; // Increment word count when exiting a word
                    inWord = 0;
                }
            } else {
                inWord = 1; // Mark start of a word
            }
        }
    }

    /* If last word was not followed by whitespace, count it */
    if (inWord) {
        words++;
    }

    /* Check for read errors */
    if (bytes_read == -1) {
        fprintf(stderr, "Error: Unable to read file %s\n", filename);
        close(fd);
        return;
    }

    /* Close the file after processing */
    close(fd);
    
    /* Print file statistics */
    printf("%d %d %d %s\n", lines, words, bytes, filename);
    
    /* Accumulate totals for multiple file processing */
    *totalLines += lines;
    *totalWords += words;
    *totalBytes += bytes;
}
