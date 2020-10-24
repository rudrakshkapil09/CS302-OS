#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define N 4

typedef struct PROCESS
{
	int pid;
	int burst,wait,turn,arrival;
} process;

process proc[N];
int time = 0;

//the inputs
int burst[N] = {5,3,8,7};
int arrival[N] = {0,1,2,3};

int getShortestjob()
{
	int ind = -1;
	int min;
	for (int i = 0; i < N; i++)
	{
		if (time >= proc[i].arrival && proc[i].burst > 0)
		{
			ind = i;
			min = proc[i].burst;
			break;
		}
	}
	if (ind == -1)
		return -1;
	
	for (int i = ind+1; i < N; i++)
	{
		if (proc[i].burst < min && proc[i].arrival <= time && proc[i].burst != -1)
		{
			ind  = i;
			min = proc[i].burst;
		}
	}
	
	return ind;
}


void incrementWaitingTime(int ind)
{
	for (int i = 0; i < N; i++)
	{
		if (i == ind || proc[i].arrival >= time || proc[i].burst == -1)
			continue;
		proc[i].wait++;	
	}
}

void incrementTurnAroundTime()
{
	for (int i = 0; i < N; i++)
	{
		if (proc[i].arrival >= time || proc[i].burst != -1)
			continue;
			
		proc[i].turn++;
	}
} 

int main()
{
	int avgWait = 0, avgTurn = 0;
	for (int i = 0; i < N; i++)
	{
		proc[i].pid = i;
		proc[i].wait = 0;
		proc[i].turn = 0;
		proc[i].arrival = arrival[i];
		proc[i].burst = burst[i];
	}
		
	int finished = 0;
	int current = -1;
	while (finished < N)
	{
		if (current == -1)
		    current = getShortestjob();
		if (current != -1)
			proc[current].burst--;
		
		incrementWaitingTime(current);
		incrementTurnAroundTime();
		if (proc[current].burst == 0)
		{
			proc[current].burst = -1;
			current = -1;
			finished++;
		}
		
		time++;
	}
	
	printf("PID\tBURST\tARRIVAL\tWAIT\tTURNAROUND\n");
	
	for(int i = 0; i < N; i++)
	{
		proc[i].burst = burst[i];
		printf("%d\t%d\t%d\t%d\t%d\n", proc[i].pid, proc[i].burst, proc[i].arrival, proc[i].wait, proc[i].turn);
		avgWait += proc[i].wait;
		avgTurn += proc[i].turn;
	}
	avgWait = avgWait/N;
	avgTurn = avgTurn/N;
	printf("\nAVERAGE WAITING TIME = %d\nAVERAGE WAITING TIME = %d\n",avgWait,avgTurn);
		
	return 0;
}
