#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
const int M = 1e4+9;
const int P = 31;

struct Data {
	int x;
};

struct File {
	int id;
	char filename[20];
	struct Data *data[10];
};

struct File files[M];

//hash function
int hash(char *c)
{
	long ans = 0, temp, p_pow = 1;
	for (int i = 0; c[i] != '\0'; i++) {
		temp = ((c[i] - 'a') * p_pow) % M;
		p_pow *= P;
		ans = (ans + temp) % M;
	}
	printf("%l", ans);
	return ans;
}

//printing
void printCharArray(char *c)
{
	for (int i = 0; c[i] != '\0'; i++)
		printf ("%c", c[i]);
}

//delete
void deleteFile(char filename[20])
{
	int ind = hash(filename);
	if (files[ind].id == -1) {
		printf("File '");
		printCharArray(filename);
		printf("' not found!\n");
	}
	else {
		files[ind].id = -1;
		printf("File '");
		printCharArray(filename);
		printf("' deleted!\n");
	}
}

//insertion
int checkExists(char filename[20])
{
	if (files[hash(filename)].id == -1)
		return 0;
	return 1;
}

void insertFile (char filename[20])
{	
	if (checkExists(filename)) {
		printf("Error: File already exists!\n");
		return;
	}
	
	int ind = hash(filename);
	files[ind].id = 1;
	strcpy(files[ind].filename, filename);

	for (int i = 0; i < 10; i++) {
		files[ind].data[i] = malloc(sizeof(struct Data));
		files[ind].data[i]->x = i;
	}
		
	printf("Inserted file -> ");
	printCharArray(filename);
	printf("\n");
}

//printing
void printAll()
{
	printf("Current Directory: ");
	for (int i = 0; i < M; i++) {
		if (files[i].id != -1) {
			printCharArray(files[i].filename);
			printf(", ");
		}
	}
	printf("\n");
}

//MAIN
int main()
{
	for (int i = 0; i < M; i++)
		files[i].id = -1;
	
	insertFile("Hello");
	insertFile("World");
	insertFile("This");
	printAll();
	insertFile("This");
	deleteFile("world");
	printAll();
	deleteFile("World");	
	printAll();
}