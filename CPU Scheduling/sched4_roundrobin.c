#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define N 4
#define tq 3
typedef struct PROCESS
{
	int pid;
	int burst,wait,turn,arrival;
} process;

process proc[N];
int time = 0, head = -1, tail = -1, current = -1;

//the inputs
int burst[N] = {5,3,8,7};
int arrival[N] = {0,1,2,3};

void updateQueue()
{
	for (int i = 0; i < N; i++)
	{
		if (arrival[i] == time)
		{
			tail++;
			if (head == -1)
				head++;
			
			proc[tail].burst = burst[i];
			proc[tail].arrival = arrival[i];
			proc[tail].pid = i + 1;
			printf("%d\n",proc[tail].pid);
			proc[tail].wait = 0;
			proc[tail].turn = 0;	
			arrival[i] = -1;
		}
	}
}


int getNextProcess()
{
	if (head == -1)
		return -1;
	current++;
	
	if (current > tail)
		current = 0;
	while (proc[current].burst == -1)
		current++;
	return current;		
		
}

void incrementWaitingTime(int ind)
{
	for (int i = 0; i <= tail; i++)
	{
		if (i == ind || proc[i].arrival >= time || proc[i].burst == -1)
			continue;
		proc[i].wait++;	
	}
}


int main()
{	 
	int avgWait = 0, avgTurn = 0;
	int finished = 0;
	int current = -1;
	while (finished < N)
	{
		updateQueue();
		int cur = getNextProcess();
		int t = tq;
		while (proc[cur].burst > 0 && t > 0)
		{
			updateQueue();
			proc[cur].burst--;
			t--;
			time++;
			incrementWaitingTime(cur);
		}
		if (proc[cur].burst == 0)
		{
			printf("Process %d has finished executing at time = %d\n",proc[cur].pid,time);
			proc[cur].burst = -1;
			proc[cur].turn = time - proc[cur].arrival;
			finished++;
		}
	}
	
	printf("\nPID\tBURST\tARRIVAL\tWAIT\tTURNAROUND\n");
	
	for (int i = 0; i < N; i++)
	{
		printf("%d\t%d\t%d\t%d\t%d\n", proc[i].pid, burst[i], proc[i].arrival, proc[i].wait, proc[i].turn);
		avgWait += proc[i].wait;
		avgTurn += proc[i].turn;
	}
	avgWait = avgWait/N;
	avgTurn = avgTurn/N;
	
	printf("\nAVERAGE WAITING TIME = %d\nAVERAGE TURNAROUND TIME = %d\n", avgWait, avgTurn);
		
	return 0;
}
