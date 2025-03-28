/* Pat Eizenga
 * Date: 3/28/25
 * Course: COSC 3355
 * Program 03
 * Purpose: This program demonstrates the use of POSIX API calls (fork, execvp, wait)
 * to create child processes that execute system commands on a specified file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 10  // Buffer size for input

// Displays interactive menu for file operations
void display_menu(const char *filename) {
    printf("\nFile: %s\n", filename);
    printf("Select an option:\n");
    printf("1. Display file contents (cat)\n");
    printf("2. Show newline, word, and character count (wc)\n");
    printf("3. Show directory details (ls -l)\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
}

// This executes system commands using fork and execvp
void execute_command(char *const args[]) {
    pid_t pid = fork(); 

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) { 
        execvp(args[0], args);
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }
    else { 
        int status;
        waitpid(pid, &status, 0); 
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    char *filename = argv[1];
    char input[BUFFER_SIZE];

    while (1) {
        display_menu(filename);
        
        // Read user input using fgets
        if (!fgets(input, BUFFER_SIZE, stdin)) {
            fprintf(stderr, "Error reading input. Exiting.\n");
            exit(EXIT_FAILURE);
        }

        // Convert input to an integer
        int choice = atoi(input);

        char *args[4] = {NULL, filename, NULL, NULL};

        switch (choice) {
            case 1:
                args[0] = "cat";
                break;
            case 2:
                args[0] = "wc";
                break;
            case 3:
                args[0] = "ls";
                args[1] = "-l";
                args[2] = filename;
                args[3] = NULL;
                break;
            case 4:
                printf("Exiting...\n");
                exit(EXIT_SUCCESS);
            default:
                printf("Invalid choice! Please try again.\n");
                continue;
        }
        
        execute_command(args);
    }
    
    return 0;
}
