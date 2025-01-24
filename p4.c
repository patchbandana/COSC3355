#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	printf("Hello, world! (pid : %d)\n", (int) getpid());
	int rc = fork();
	if (rc < 0) {	//fork failed; exit
		fprintf(stderr, "Fork failed!\n");
		exit(1);
	} else if (0 == rc) {	// child (new process)
		printf("Hello, I am the child process. (pid: %d)\n", (int) getpid());
		char *myargs[3];
		myargs[0] = malloc(3 * sizeof(char));
		strncpy(myargs[0], "wc", 3);
		myargs[1] = malloc(5 * sizeof(char));
		strncpy(myargs[1], "p4.c", 5);
		myargs[2] = NULL;
		execvp(myargs[0], myargs);
		printf("This should not print.");

	} else {	// parent process
		 int rc_wait = wait(NULL);
		 printf("Hello, I am the parent of %d (rc_wait: %d) (pid: %d)\n", rc,
				rc_wait, (int) getpid());
}
return 0;
}

