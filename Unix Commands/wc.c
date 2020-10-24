#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int args, char **argv)
{
    FILE * file;
    char ch;
    int totalchar = 0, totalwords = 0, totallines = 0;

	int i = 1;
	while (i < args) {
	    char path[100] = "/home/cs17/c177154/";
    	int characters = 0, words = 0, lines = 0;	
		strcat(path, argv[i]);

		file = fopen(path, "r");

	    if (file == NULL)
	    {
	        printf("ERROR\n");
			continue;
	    }

    	while ((ch = fgetc(file)) != EOF)
    	{
        	characters++;
        	switch(ch) {
			case '\0': 
			case '\n':
				lines++;
			case ' ':
			case '\t':
				words++;
			default: break;		
			}
	    }
	
	    printf(" %d\t", lines);
	    printf("%d\t", words);
	    printf("%d\t", characters);
	    printf("%s\n", argv[i]);

    	fclose(file);
    	totalchar += characters;
    	totalwords += words;
    	totallines += lines;

		i++;
	}
	
	if (args <= 2)
		return 0;	

	printf(" %d\t", totallines);
    printf("%d\t", totalwords);
    printf("%d\t", totalchar);
    printf("TOTAL\n");
    return 0;
}
