//each 

#include <stdlib.h>
#include <stdio.h>
const int MEMORYSIZE = 5;
const int MAXJOBS = 10;
const int N = 5;

struct Block {
	int id; 
	int available;
};

struct Memory {
	int id;
	struct Block blocks[N-1];
	struct Memory *next;
};

struct Job {
	int id;
	struct Block *block;
};

struct Job jobs[MAXJOBS];
struct Block dummy;
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
	head->id = 0;
	for (int i = 0; i < N-1; i++) {
		head->blocks[i].id = i;
		head->blocks[i].available = 1;
	}
	tail = head;
	for (int i = 1; i < MEMORYSIZE; i++) {
		tail->next = malloc(sizeof(struct Memory));
		tail = tail->next;
		tail->id = i;
		for (int i = 0; i < N-1; i++) {
			tail->blocks[i].id = i;
			tail->blocks[i].available = 1;
		}
	}
	tail->next = NULL;
}

struct Block* getNextBlock(struct Memory *mem, int memNum)
{
	if (mem == NULL)
		printf("OUT OF MEMORY");
	else {
		for (int i = 0; i < N-1; i++) {
			if (mem->blocks[i].available) {
				mem->blocks[i].available = 0;
				printf("Allocated block %d of memory %d", i, memNum);
				return &mem->blocks[i];
			}
		}
		memNum++;
		getNextBlock(mem->next, memNum);
		
	}
	return NULL;
}

//insert job into back of Q and give memory to it
int allocate()
{
	int memNum = 0;
	jobs[back].id = counter++;
	jobs[back].block = getNextBlock(head, memNum);
	if (jobs[back].block == NULL)
		return 0;
		
	printf(" to job %d\n", jobs[back].id);
	back++;
	return 1;
}

void deallocate()
{
	jobs[front].block->available = 1;
	printf("Job %d released block %d\n", jobs[front].id, jobs[front].block->id);
	front++;
}

int main()
{
	initialise();
	
	for (int i = 0; i < MAXJOBS; i++) {
		if (allocate() == 0) {
			break;
		}
		int p = rand() % 2;
		if (p % 2 == 0)				//50% chance to deallocate 
			deallocate();
	}
	
	return 0;
}
