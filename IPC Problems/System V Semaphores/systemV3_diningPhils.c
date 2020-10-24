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

#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N

struct sembuf p = { 0, -1, SEM_UNDO};   // WAIT
struct sembuf v = { 0, +1, SEM_UNDO};   // SIGNAL

//shared memory - queue 
struct smph
{
    int state[N];
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

void test(struct smph *shm, int semid, int phnum) {
    if (shm->state[phnum] == HUNGRY && shm->state[LEFT] != EATING && shm->state[RIGHT] != EATING) {
        shm->state[phnum] = EATING;
        printf("Philosopher %d takes fork %d and %d\n",
                      phnum + 1, LEFT + 1, phnum + 1);
        printf("Philosopher %d is Eating\n", phnum + 1);
  
        Signal(semid, phnum+1);
    }
}

// take up chopsticks
void take_fork(int semid, struct smph *shm, int phnum) {
    Wait(semid, 0);
  
    // state that hungry
    shm->state[phnum] = HUNGRY;
    printf("Philosopher %d is Hungry\n", phnum + 1);
    
    // eat if neighbours are not eating
    test(shm, semid, phnum);
    Signal(semid, 0);
    
    // if unable to eat wait to be signalled
    Wait(semid, phnum+1);
}

// put down chopsticks
void put_fork(int semid, struct smph *shm, int phnum) {
    Wait(semid, 0); //wait for mutex
    
    // state that thinking
    shm->state[phnum] = THINKING;
    printf("Philosopher %d putting fork %d and %d down\n",
           phnum + 1, LEFT + 1, phnum + 1);
    printf("Philosopher %d is thinking\n", phnum + 1);
  
    //test left and right sticks
    test(shm, semid, LEFT);
    test(shm, semid, RIGHT);
  
    Signal(semid, 0);   //signal mutex
}


void* Philosophers(int semid, struct smph* shm, int phnum) {
    while (1) {
        take_fork(semid, shm, phnum);
        sleep(1);
        put_fork(semid, shm, phnum);
    }
}


//MAIN
int main()
{
    printf("\n\nSTART:\n\n");
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
    for (int i = 0; i < N; i++)
        shm->state[i] = THINKING;
    
    //initialise first semaphore - mutex -> 1
    union semun u;
    u.val = 1;
    semid = semget(key, 3, 0666 | IPC_CREAT);
    semctl(semid, 0, SETVAL, u);
    
    //init others -> S[i-1]
    for (int i = 1; i <= N; i++) {
        u.val = 0;
        semctl(semid, i, SETVAL, u);
    }
    
    //create N child processes and run functions concurrently
    for (int i = 0; i < N; i++) {
        int pid = fork();
        if (pid == 0) {
            Philosophers(semid, shm, i);
			exit(0);
        }
    }
	
	//wait for them to all finish
	for (int i = 0; i < N; i++)
		wait(NULL);

    
    return 0;
}


