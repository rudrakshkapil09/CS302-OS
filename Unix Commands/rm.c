//rm
#include <stdio.h>
#include <string.h>

int main(int args, char **argv) {
	int iflag = 0, fflag = 0;
	//check flags
	for (int i = 1; i < args; i++) {
		if (strcmp(argv[i], "-i") == 0)
			iflag = 1;
		if (strcmp(argv[i], "-f") == 0)
			fflag = 1;
	}
	//f doesnt allow any prompts
	if (fflag) 
		iflag = 0;

	if (args < 1) {
		printf("ERROR\n");	
		return 0;
	}
	
	//multiple files
	int i = 1;
	while (i < args) {
		if (argv[i][0] == '-') 
			continue;
		if (fflag == 1) {
			remove(argv[i]);
			continue;
		}
		
		if (iflag == 1) {
			char choice;
			printf(" remove %s? ", argv[1]);
			scanf("%c", &choice);
			
			if (choice == 'N' || choice == 'n')
				continue;
		}

		if (remove(argv[i]) != 0)
			printf("Could not delete!");	
	}
}
