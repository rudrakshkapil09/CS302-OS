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

		while ((ch = fgetc(file)) != EOF) {
			if (ch == '\n') 
				count++;		
		}
		fclose(file);

		file = fopen(path, "r");
		int count2 = 0;
		while ((ch = fgetc(file)) != EOF) {
			if (count2 >= count-10)
				printf("%c", ch);
			if (ch == '\n') 
				count2++;		
		}

		fclose(file);
	}
	return 0;
}
