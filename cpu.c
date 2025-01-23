//Pat Eizenga
//Chapter 2.1 Notes
//Fixed broken example using nonfictional functions/very first C program
//1/22/25

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>
#include <unistd.h>

int
main (int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "usage: cpu <string>\n");
		exit(1);
	}
	char *str = argv[1];
	while (1) {
		sleep(1);
		printf("%s\n", str);
	}
	return 0;
}

