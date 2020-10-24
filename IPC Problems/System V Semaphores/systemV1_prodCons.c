#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define N 10
#define NITERS 40

struct sembuf p = { 0, -1, SEM_UNDO};   // WAIT
struct sembuf v = { 0, +1, SEM_UNDO};   // SIGNAL

//shared memory - queue 
struct smph
{
	int buf[N];
	int in;
	int out;
};

//fourth argument for semctl
/*
union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
};
*/

//Wait
void Wait(int semid, int semindex)
{
	p.sem_num = semindex;
	semop(semid, &p, 1);
}

//Signal
void Signal(int semid, int semindex)
{
	v.sem_num = semindex;
	semop(semid, &v, 1);
}

//producer:
void *Producers(int semid, struct smph *shm) {
	for (int i = 0; i < NITERS; i++) {
		int item = i;
	
		//entry
		Wait(semid, 0);		//wait empty
		Wait(semid, 2);		//wait mutex
	
		//CS:
		shm->buf[shm->in] = item;
		printf("Produced item {%d} at position [%d]\n", item, shm->in);
		shm->in = (shm->in+1) % N;
		sleep(1);
	
		//exit
		Signal(semid, 2);	//signal mutex
		Signal(semid, 1);	//signal full
	}
	
}

//consumer:
void *Consumers(int semid, struct smph *shm) {
	for (int i = 0; i < NITERS; i++) {
		int item = 1;
	
		//entry
		
		Wait(semid, 1);		//wait full
		Wait(semid, 2);		//wait mutex
		
		//CS:
		item = shm->buf[shm->out];
		printf("Consumed item {%d} from position [%d]\n", item, shm->out);
		shm->out = (shm->out+1) % N;
		sleep(1);
	
		//exit
		Signal(semid, 2);	//signal mutex
		Signal(semid, 0);	//signal empty
	}
}

int main()
{
	//variables
	int shmid,semid, key;
	struct smph * shm;
	
	//get semaphores - print error if necessary
	shmid = shmget(key, sizeof(struct smph), IPC_CREAT|0660);
	if (shmid == -1)
		perror("Shared Memory fault\n");
	
	//link to shm pointer
	shm = shmat(shmid, NULL, 0);
	if (shm == (void *) -1)
		perror("Attachment fault\n");
	
	//initialise pointer
	shm->in=0;
	shm->out=0;
	
	//initialise first semaphore - empty -> N
	union semun u;
	u.val = N;	
	semid = semget(key, 3, 0666 | IPC_CREAT);	
	semctl(semid, 0, SETVAL, u); 
	
	//init second - full -> 0
	u.val = 0;
	semctl(semid, 1, SETVAL, u);
	
	//init third - mutex -> 1
	u.val = 1;
	semctl(semid, 2, SETVAL, u);
	
	//create child process and run functions concurrently
	int pid = fork();
	if (pid == 0) {	//child
		//shm = shmat(shmid, NULL, 0);
		Producers(semid, shm);
		exit(0);
		wait(NULL);
	}
	else {
		Consumers(semid, shm);
	}


	
	return 0;
}


