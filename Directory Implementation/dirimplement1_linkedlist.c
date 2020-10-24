#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Data {
	int x;
};

typedef
struct File {
	char filename[20];
	struct Data *data[10];
	struct File *next;
} *FPTR;
FPTR head;


//printing
void printCharArray(char *c)
{
	for (int i = 0; c[i] != '\0'; i++)
		printf ("%c", c[i]);
}


//insertion
int checkExists(FPTR F, char filename[20])
{
	if (F == NULL)
		return 0;
	else if (strcmp(F->filename, filename) == 0)
		return 1;
	else return checkExists(F->next, filename);
}

void insertFile (char filename[20])
{	
	if (checkExists(head, filename)) {
		printf("Error: File '");
		printCharArray(filename);
		printf("' already exists!\n");
		return;
	}
	
	FPTR T = malloc(sizeof(struct File));
	T->next = NULL;
	strcpy(T->filename, filename);
	for (int i = 0; i < 10; i++) {
		T->data[i] = malloc(sizeof(struct Data));
		T->data[i]->x = i;
	}
	
	if (head != NULL) {
		FPTR X = head;
		while (X->next)
			X = X->next;
		X->next = T;
	}
	else head = T;
	printf("Inserted file -> ");
	printCharArray(filename);
	printf("\n");
}

//deleting
void deleteFile(char *c)
{
	FPTR T = head;
	//first item
	if (strcmp(T->filename, c) == 0) {
		head = T->next;
		printCharArray(c);
		printf(" deleted!\n");
	}
	
	while (T != NULL && T->next != NULL) {
		if (strcmp(T->next->filename, c) == 0) {
			T->next = T->next->next;
			printCharArray(c);
			printf(" deleted!\n");
			return;
		}
		T = T->next;
	}
	if (T->next != NULL) {
		if (strcmp(T->next->filename, c) == 0) {
			T->next = NULL;
			printCharArray(c);
			printf(" deleted!\n");
			return;
		}
	}
	
	printf("File '");
	printCharArray(c);
	printf("' not found!\n");
}

//printing
void printAll()
{
	FPTR T = head;
	if (T == NULL)
		return;
	printf("Current Directory: ");
	while (T->next) {
		printCharArray(T->filename);
		printf(" -> ");
		T = T->next;
	}
	printCharArray(T->filename);
	printf("\n");
}

//MAIN
int main()
{
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