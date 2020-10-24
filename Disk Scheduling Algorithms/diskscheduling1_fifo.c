#include <stdio.h>
#include <stdlib.h>

int main()
{
     int queue[100], n, head, i, j, seek = 0, diff;
     float avg;
     printf("Enter the size of queue: ");
     scanf("%d",&n);
     printf("Enter the queue: ");
	 
	 for (i = 1; i <= n; i++)
     	scanf("%d", &queue[i]);
	 
	 printf("Enter the initial head position: ");
     scanf("%d", &head);
     queue[0] = head;
     printf("\n");
	 
     for (j = 0; j <= n-1; j++)
     {
     	diff = abs(queue[j+1] - queue[j]);
        seek += diff;
        printf("Move from %d to %d with Seek %d\n", queue[j], queue[j+1], diff);
     }
	 
     printf("\nTotal Seek Time   => %d\t", seek);
     avg = seek/(float)n;
     printf("\nAverage Seek Time => %f\t", avg);
	 return 0;
}