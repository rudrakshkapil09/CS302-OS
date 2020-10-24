#include <stdlib.h>
#include <stdio.h>
const int totalBlocks = 2, totalWords = 2;
long long space[totalWords];

struct Job {
	int w;
	int b;
};
struct Job jobs[totalBlocks * totalWords];
int tail = 0;
int front = 0;

void print()
{
	printf("\n---------------\n");
	for (int i = 0; i < totalWords; i++) {
		long long temp = space[i];
		char s[totalBlocks];
		for (int j = 0; j < totalBlocks; j++) {
			s[totalBlocks-j-1] = (temp & 1) + '0';
			temp = temp >> 1;
		}
			
		for (int j = 0; j < totalBlocks; j++)
			printf("%c", s[j]);
		
		printf("\n");
	}
	printf("---------------\n");
}

//find first free block
int allocate()
{
	for (int i = 0; i < totalWords; i++) {
		if (space[i] == 0)
			continue;
		for (int j = totalBlocks-1; j >= 0; j--) {
			if (space[i] & (1 << j)) {	//free
				space[i] &= ~(1 << j);
				jobs[tail].w = i;
				jobs[tail++].b = j;
				return 1;
			}
		}
	}
	return 0;
}

void deallocate(int word, int block)
{
	space[word] |= (1 << block);
}

int main()
{
	for (int i = 0; i < totalWords; i++)
		space[i] = 0;

	for (int i = 0; i < totalWords; i++)
		for (int j = 0; j < totalBlocks; j++)
			space[i] |= (1 << j);
	
	//allocate - loop breaks if all allocated
	print();
	int count = 30;
	
	while (count-- > 0) {
		if (allocate() == 0)
			break;
		print();
		int p = rand() % 2;
		if (p % 2 == 0) {				//50% chance to deallocate
			deallocate(jobs[front].w, jobs[front].b);
			front++;
			print();
		}
	}
	
	return 0;
}
