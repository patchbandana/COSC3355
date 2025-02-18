/*  Pat Eizenga
 *  COSC3355.001
 *  2/12/2025
 *  Purpose: A wordcount program that takes a file path and
 *   outputs the number of lines, words, and bytes for each
 *   file using limited libraries and POSIX API calls.
 *   Sources used: Stack Exchange for 4k Buffer Size in Unix systems
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

// Hard coded limit for Linux command line buffer
#define BUFFER_SIZE 4096

// Function for processing a file
void process_file(const char *filename, int *totalLines, int *totalWords, int *totalBytes);

int main(int argc, char *argv[])
{
	//Ensure the program has at least one file argument in the command line
    if (argc < 2) {
        fprintf(stderr, "Please enter: %s <file1> <file2> ...\n", argv[0]);
	//Untrained user goes ????
        return 1;
    }

    // Initialize counters for total counts across all files
    int totalLines = 0;
    int totalWords = 0;
    int totalBytes = 0;
    //   Iterate through each file passed as a command line argument
    for (int i = 1; i < argc; i++) {
        process_file(argv[i], &totalLines, &totalWords, &totalBytes);
    }

	// If more than one file is processed, print a total
    if (argc > 2)
    {
        printf("%d %d %d total\n", totalLines, totalWords, totalBytes);
    }
    return 0;
}

void process_file(const char *filename, int *totalLines, int *totalWords, int *totalBytes)
{
    // Attempt to open the file, read only!
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        fprintf(stderr, "Error 404: file %s not found!\n", filename);
        return;
    }
	// Buffer to hold file data during read with linux approved number
    char buffer[BUFFER_SIZE];
    // Stores number of bytes
    ssize_t bytes_read;
    // Counters
    int lines = 0;
    int words = 0;
    int bytes = 0;
    //Are we in a word? Yes or no?
    int wordFlag = 0;

    //Read the file in chunks till end
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
	    // Accumulator
        bytes += bytes_read;
        for (ssize_t i = 0; i < bytes_read; i++)
       	{
            if (buffer[i] == '\n')
	    {
	 // Increment line counter when you see the newline escape character
                lines++;
            }
            if (buffer[i] == ' ' || buffer[i] == '\t' || buffer[i] == '\n')
	    {
                wordFlag = 0;
		// End word
            } else if (!wordFlag)
	    {
		    // Begin word
                wordFlag = 1;
                words++;
            }
        }
    }

    //Input validation, in Java this would be handled by IOException, I think
    if (bytes_read == -1)
    {
        fprintf(stderr, "Error: Unable to read file %s\n", filename);
        close(fd);
        return;
    }

    // Close file
    close(fd);
    // Print file according to assignment 02
    printf("%d %d %d %s\n", lines, words, bytes, filename);
    // Accumulators if more than one file
    *totalLines += lines;
    *totalWords += words;
    *totalBytes += bytes;
}
