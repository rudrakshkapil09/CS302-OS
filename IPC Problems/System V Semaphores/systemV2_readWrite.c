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

int readcount = 0, writecount = 0;

struct sembuf p = { 0, -1, SEM_UNDO};   // WAIT
struct sembuf v = { 0, +1, SEM_UNDO};   // SIGNAL

//fourth argument for semctl
/*
union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
};*/

//shared memory - queue 
struct smph
{
	int sh_var;
}; 

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

void *Reader(int semid, struct smph *shm, int i)
{
    printf("\nReader-%d is reading...", i);

	//entry:
	Wait(semid, 3);			//wait readTry
    Wait(semid, 0);			//wait readMutex
    readcount++;
    if(readcount == 1)		//first reader
        Wait(semid, 1);		//wait resource
    Signal(semid, 0);		//signal readMutex
    Signal(semid, 3);		//signal readTry
    
    //CS:
    printf("\nCurrent value: %d\n", shm->sh_var);
    
	//exit:
    Wait(semid, 0);				//wait readMutex
    readcount--;
    if(readcount==0)			//last reader
        Signal(semid, 1);		//signal resource
    Signal(semid, 0);			//signal readMutex
}

void *Writer(int semid, struct smph *shm, int i)
{
    printf("\nWriter-%d is writing... (+5)", i);

	//entry:
    Wait(semid, 2);				//wait writeMutex
    writecount++;
    if(writecount == 1)			//first writer locks out readers
        Wait(semid, 3);			//wait readTry
    Signal(semid, 2);			//signal writeMutex

	//CS:
    Wait(semid, 1);				//wait resource
    shm->sh_var += 5;	
    Signal(semid, 1);				//signal resource

	//exit:
    Wait(semid, 2);					//wait writeMutex
    writecount--;
    if(writecount == 0)				//last writer unlocks readers
        Signal(semid, 3);				//signal readTry
    Signal(semid, 2);					//signal writeMutex
}


//MAIN
int main()
{
    printf("\nSTART:\n");
    //variables
    int shmid,semid;
 	key_t key;
 	
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
	shm->sh_var = 5;
    
    //initialise all semaphores -> 1
    //0->readMutex, 1->resource, 2->writeMutex, 3->readTry
    union semun u;
    u.val = 1;
    semid = semget(key, 3, 0666 | IPC_CREAT);
    for (int i = 0; i < 4; i++)
        semctl(semid, i, SETVAL, u);

    //create N child processes and run functions concurrently
    for (int i = 1; i <= 7; i++) {
        int pid = fork();
        if (pid == 0) {
            if (i % 2)
				Reader(semid, shm, i);
        	else Writer(semid, shm, i);
        	exit(0);
        }
    }
    
    for (int i = 1; i <= 7; i++)
    	wait(NULL);
    
    
    return 0;
}




