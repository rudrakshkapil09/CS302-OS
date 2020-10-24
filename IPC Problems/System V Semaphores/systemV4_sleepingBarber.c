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

#define MAX_CUSTOMERS 50

struct sembuf p = { 0, -1, SEM_UNDO};   // WAIT
struct sembuf v = { 0, +1, SEM_UNDO};   // SIGNAL

//shared memory - queue 
struct smph
{
	int allDone;
};

//fourth argument for semctl
/*
union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
};*/

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

//customer
void *Customer(int semid, int number) {
    printf("Customer %d arrived at barber shop.\n", number);
    Wait(semid, 0);									// wait for waiting room spot
    
    printf("Customer %d entered waiting room.\n", number);
    sleep(1);
    Wait(semid, 1);									// wait for chair to be empty
	printf("Customer %d sat in barber's chair.\n", number);
	sleep(1);
    Signal(semid, 0);   								// signal free spot in waiting room.

    Signal(semid, 2);								// signal to wake up the barber
    printf("Customer %d woke the barber.\n", number);
    sleep(1);

    Wait(semid, 3);									// Wait for the barber to finish cutting your hair.
	printf("Customer %d done with haircut.\n", number);
	sleep(1);

    Signal(semid, 1);									// signal that barber chair is empty
    printf("Customer %d leaving barber shop.\n", number);
	sleep(1);
}

//barber:
void *Barber(int semid, struct smph * shm) {
	//while customers are still there
    while (!shm->allDone) {
        printf("The barber is sleeping.\n");					// Sleep until someone arrives and wakes you
        Wait(semid, 2);
		printf("The barber is awake.\n");

        if (!shm->allDone) {
			//cut hair for some amount of time
            printf("The barber is cutting hair\n");
            sleep(2);
            printf("The barber has finished cutting hair.\n");

            Signal(semid, 3);            					// Release the customer when done cutting
        }
        else printf("The barber is going home for the day.\n");
    }
}

//MAIN
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
	shm->allDone = 0;
	
	//get input
	int i, numCustomers, numChairs;
    printf("Enter the number of Customers : "); 
	scanf("%d",&numCustomers) ;
    printf("Enter the number of Chairs In Waiting Room : "); 
	scanf("%d",&numChairs);
	
	//initialise first semaphore - waitingroom -> N
	union semun u;
	u.val = numChairs;	
	semid = semget(key, 3, 0666 | IPC_CREAT);	
	semctl(semid, 0, SETVAL, u); 
	
	//init second - barberChair -> 1
	u.val = 1;
	semctl(semid, 1, SETVAL, u);
	
	//init third - barberPillow -> 0
	u.val = 0;
	semctl(semid, 2, SETVAL, u);
	
	//init fourth - seatbelt -> 0
	u.val = 0;
	semctl(semid, 3, SETVAL, u);
	
	
	//create child process and run functions concurrently
	int pid = fork();
	if (pid == 0) {	
		//run customers
		for (int i = 1; i <= numCustomers; i++) {
			if (fork() == 0) {
				Customer(semid, i);
				exit(0);
			}
		}

		for (int i = 0; i < numCustomers; i++)
			wait(NULL);
			
		//wake up barber at the end
		shm->allDone = 1;
		Signal(semid, 2);
	}
	else Barber(semid, shm);		//run barber
	
	return 0;
}









