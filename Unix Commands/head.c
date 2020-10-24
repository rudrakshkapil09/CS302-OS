#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int args, char **argv) 
{
	char ch;
	FILE *file;
	
	int i = 1;
	while (i < args) {
		if (args > 2)
			printf("==> %s <==\n", argv[i]);
		char path[100] = "/home/cs17/c177154/";
		strcat(path, argv[i++]);
		file = fopen(path, "r");
		if (file == NULL) {
	        printf("ERROR\n");
			continue;
	    	}
	
		int count = 0;
		while ((ch = fgetc(file)) != EOF && count < 10) {
			if (ch == '\n') 
				count++;		
			printf("%c", ch);
		}
		fclose(file);
	}
	return 0;
}
