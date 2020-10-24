#include<stdio.h>
#include<sys/types.h>
#include<signal.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("usage: ./kill PID\n");
        return -1;
    }

	int i = 1;
	while (i < argc)
    	kill(atoi(argv[i]), SIGKILL);
    return 0;
}
