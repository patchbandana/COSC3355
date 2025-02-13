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

void process_file(const char *filename, int *totalLines, int *totalWords, int *totalBytes);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> [file2 ...]\n", argv[0]);
        return 1;
    }

    int totalLines = 0;
    int totalWords = 0;
    int totalBytes = 0;
    for (int i = 1; i < argc; i++) {
        process_file(argv[i], &totalLines, &totalWords, &totalBytes);
    }

    if (argc > 2) {
        printf("%d %d %d total\n", totalLines, totalWords, totalBytes);
    }
    return 0;
}

void process_file(const char *filename, int *totalLines, int *totalWords, int *totalBytes) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Error 404: file %s not found!\n", filename);
        return;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    int lines = 0;
    int words = 0;
    int bytes = 0;
    int wordFlag = 0;

    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        bytes += bytes_read;
        for (ssize_t i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n') {
                lines++;
            }
            if (buffer[i] == ' ' || buffer[i] == '\t' || buffer[i] == '\n') {
                wordFlag = 0;
            } else if (!wordFlag) {
                wordFlag = 1;
                words++;
            }
        }
    }

    if (bytes_read == -1) {
        fprintf(stderr, "Error: Unable to read file %s\n", filename);
        close(fd);
        return;
    }

    close(fd);
    printf("%d %d %d %s\n", lines, words, bytes, filename);
    *totalLines += lines;
    *totalWords += words;
    *totalBytes += bytes;
}
