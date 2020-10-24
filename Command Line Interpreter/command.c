#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

char *input = NULL;
size_t capline = 0;

int i;
char *token;
char *array[512];

void startDisplay()
{
	printf("\n\nENTER YOUR COMMANDS BELOW:\n");
	printf("(To exit type q)\n\n");
}

void displayPrompt()
{
	printf("SHELL : ");
}

void makeTokens(char *input)
{
	i = 0;
	token = strtok(input, "\n ");

	while (token != NULL) {
		array[i++] = token;
		token = strtok(NULL, "\n ");
	}
	array[i] = NULL;
}

void execute()
{
	int pid = fork();
	if (pid != 0) {
		int s;
		waitpid(-1, &s, 0);
	}
	else {
		char comm[50] = "./";
		strcat(comm, array[0]);
		if (execvp(comm, array) == -1) {
			perror("Wrong command");
			exit(errno);
		}
	}
}

int main()
{
	startDisplay();
	while (1) {
		displayPrompt();
		getline(&input, &capline, stdin);
		if (strcmp(input, "\n") == 0) {
			perror("Please type in a command ");
			continue;
		}
		
		makeTokens(input);
		if (strcmp(array[0], "q") == 0) {
			printf("\nSYSTEM: EXIT\n\n");
			return 0;
		}
		execute();
	}
}
