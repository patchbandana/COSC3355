//	Pat Eizenga
//	COSC3355.001
//	Dr. Nix
//	1-23-2025
//	Purpose: To capture a user input series of words and return the number of words given,
//	while practicing using vim, linux command line, virtual machines, and learning C.


#include <stdio.h>
#include <string.h>
#include <ctype.h>

//Declare static constant (given by assignment)
#define MAX_INPUT_SIZE 1024

int main() {
	//Strings are arrays of characters in C.
	char input[MAX_INPUT_SIZE];
	int word_count = 0;
	int wordFlag = 0;
	//This flag signals if we are inside a word

	//Prompt the user for input
	printf("Enter a list of words separated by one or more spaces:\n");
	//fgets is used in C rather than cin/cout and stdin rather than scanner
	if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
		printf("Error reading input.\n");
		return 1;
		//Return 1 to show that program did not execute successfully
	}

	//Process the input array, counting each word.
	for (int i = 0; input[i] != '\0'; i++) {
		//When encountering a character that isn't a space, step through the array
		if (!isspace(input[i])) {
			if (!wordFlag) {
				//increment Word Count
				word_count++;
				wordFlag = 1;
			}
		}
			//If there is a space, change this sentinel value to indicate the need to increment the word count.
			else {
				wordFlag = 0;
			}
		}

		//Outputs word count
		printf("Number of words entered: %d\n", word_count);

		//Return 0 if program ran succesfully
		return 0;
	}
