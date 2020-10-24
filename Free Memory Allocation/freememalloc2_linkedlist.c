#include <stdlib.h>
#include <stdio.h>
const int MEMORYSIZE = 5;
const int MAXJOBS = 10;

struct Memory {
	int id;
	struct Memory *next;
};

struct Job {
	int id;
	struct Memory *allocated;
};

struct Job jobs[MAXJOBS];
struct Memory *head = NULL, *tail = NULL;
int front = 0, back = 0, counter = 0;


void print()
{
	struct Memory *m = head;
	if (m == NULL)
		return;
	printf("Current Memory: ");
	while (m->next) {
		printf("%d -> ", m->id);
		m = m->next;
	}
	printf("%d\n", m->id);
}

//get 10 memory spaces
void initialise()
{
	head = malloc(sizeof(struct Memory));
	//head->id = 0;
	tail = head;
	for (int i = 1; i < MEMORYSIZE; i++) {
		tail->next = malloc(sizeof(struct Memory));
		tail = tail->next;
		tail->id = i;
	}
	tail->next = NULL;
}


//insert job into back of Q and give head of memory to it
int allocate()
{
	jobs[back].id = counter++;
	jobs[back].allocated = head;
	if (head == NULL) {
		printf("MEMORY FULL\n");
		return 0;
	}
	head = head->next;
	jobs[back].allocated->next = NULL;
	
	printf("Allocated memory %d to job %d\n", jobs[back].allocated->id, jobs[back].id);
	back++;
	print();
	return 1;
}

//add allocated to tail, then pop from q
void deallocate()
{
	tail->next = jobs[front].allocated;
	tail = tail->next;
	printf("Job %d released memory %d\n", jobs[front].id, tail->id);
	front++;
	print();
}

int main()
{
	initialise();
	print();
	
	for (int i = 0; i < MAXJOBS; i++) {
		if (allocate() == 0) {
			//deallocate();
			break;
		}
		int p = rand() % 2;
		if (p % 2 == 0)				//50% chance to deallocate 
			deallocate();
	}
	
	return 0;
}
