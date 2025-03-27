/* Pat Eizenga
 * Date: 3/26/24
 * Course: COSC 3355
 * Program 03
 * Purpose: File Utility Program using POSIX Process Management
 * 
 * This program demonstrates the use of POSIX API calls (fork, execvp, wait)
 * to create child processes that execute system commands on a specified file.
 */

 #include <stdio.h>      // Standard input/output functions
 #include <stdlib.h>     // Standard library functions (e.g., exit)
 #include <unistd.h>     // POSIX operating system API (fork, execvp)
 #include <sys/types.h>  // System types (e.g., pid_t)
 #include <sys/wait.h>   // Waiting for child process functions
 
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
 // Parent process waits for child process to return using waitpid
 // Fork returns errors if pid is negative, searches path if 0
 
 void execute_command(char *const args[]) {
     pid_t pid = fork(); 
 
     if (pid < 0) {
         // Fork failed - print error and exit
         perror("fork failed");
         exit(EXIT_FAILURE);
     }
     else if (pid == 0) { 
         execvp(args[0], args);
 
         // execvp only returns if it fails
         perror("execvp failed"); // Print error details
         exit(EXIT_FAILURE);
     }
     else { 
         // Parent process
         int status;
         // Wait for the child process to complete
         // Blocks parent until child terminates
         waitpid(pid, &status, 0); 
     }
 }
 
 //Main MASTER! B)
 int main(int argc, char *argv[]) {
     // Validate number of command-line arguments
     if (argc != 2) {
         // Incorrect usage - print error to stderr and exit
         fprintf(stderr, "Actually, do %s <filename>\n", argv[0]);
         exit(EXIT_FAILURE);
     }
     
     // Store filename from command-line argument
     char *filename = argv[1];
     int choice;
     
     // Infinite loop for menu
     while (1) {
         // Display menu with current filename
         display_menu(filename);
 
         // Read user's menu choice
         // Check if input is valid integer
         if (scanf("%d", &choice) != 1) { 
             fprintf(stderr, "Invalid input. Exiting.\n");
             exit(EXIT_FAILURE);
         }
         
         // Prepare argument array for system commands
         // Initialize with NULL to ensure proper argument termination
     //It's cool knowing what args actually means now
         char *args[3] = {NULL, filename, NULL};
         
         // Select command based on user's menu choice
         switch (choice) {
             case 1:
                 // Display file contents using 'cat'
                 args[0] = "cat";
                 break;
             case 2:
                 // Show file statistics using 'wc'
                 args[0] = "wc";
                 break;
             case 3:
                 // Show file directory details using 'ls -l'
                 args[0] = "ls";
                 args[1] = "-l";
                 args[2] = filename;
                 args[3] = NULL;
                 break;
             case 4:
                 // Exit the program
                 printf("Exiting...\n");
                 exit(EXIT_SUCCESS);
             default:
                 // Handle invalid menu choice
                 printf("Invalid choice! Do better!\n");
                 continue;
         }
         
         // Execute the selected command
         execute_command(args);
     }
     
     return 0;
 }
 