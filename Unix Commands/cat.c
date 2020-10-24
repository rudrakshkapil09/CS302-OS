#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int args, char **argv) 
{
	char ch;
	FILE *file;
	
	int i = 1;
	
	//multiple files
	while (i < args) {
		char path[100] = "/home/cs17/c177154/";
		strcat(path, argv[i++]);
		file = fopen(path, "r");
		if (file == NULL) {
	        printf("ERROR\n");
			continue;
	    	}
	
		while ((ch = fgetc(file)) != EOF) {
			printf("%c", ch);
		}
		fclose(file);
	}
	return 0;
}
