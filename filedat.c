/* Pat Eizenga
 *  3/26/24
 *  Program 03
 *  Purpose: This program uses the fork, wait, API POSIX calls to implement the
 * functionality of the previous progrms. Fork calls a child process and 
 */   

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void display_menu(const char *filename) {
    printf("\nFile: %s\n", filename);
    printf("Select an option:\n");
    printf("1. Display file contents (cat)\n");
    printf("2. Show newline, word, and character count (wc)\n");
    printf("3. Show directory details (ls -l)\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
}

void execute_command(char *const args[]) {
    pid_t pid = fork(); // Create a child process
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) { // Child process
        execvp(args[0], args);
        perror("execvp failed"); // If execvp fails
        exit(EXIT_FAILURE);
    }
    else { // Parent process
        int status;
        waitpid(pid, &status, 0); // Wait for child to complete
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    char *filename = argv[1];
    int choice;
    
    while (1) {
        display_menu(filename);
        if (scanf("%d", &choice) != 1) { // Read user input
            fprintf(stderr, "Invalid input. Exiting.\n");
            exit(EXIT_FAILURE);
        }
        
        char *args[3] = {NULL, filename, NULL};
        
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
                printf("Invalid choice. Try again.\n");
                continue;
        }
        
        execute_command(args);
    }
    
    return 0;
}
