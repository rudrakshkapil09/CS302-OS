#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define qptr PQ*
#define MAXSIZE 1024
#define MAXBURSTTIME 10

struct Process{
	int burst_time , p_id;
};

pthread_mutex_t mut;

struct PQ{
	struct Process proc;	
	struct qptr next;
	struct qptr prev;
};

struct qptr myQueue = NULL;
struct qptr tail = NULL;
 
void createProcess(struct Process* param) {
	struct Process * thisProcess = param; 
	
	if (myQueue == NULL) {
		myQueue = (struct qptr) malloc(sizeof(struct PQ));
		myQueue->proc.burst_time = thisProcess->burst_time;
		myQueue->proc.p_id = thisProcess->p_id;	
		myQueue->next = NULL;
		myQueue->prev = NULL;
		tail = myQueue;
	}
	else {
		struct qptr temp = (struct qptr)malloc(sizeof(struct PQ));
		temp->proc.burst_time = thisProcess->burst_time;
		temp->proc.p_id = thisProcess->p_id;	
		temp->next = NULL;
		tail->next = temp;
		temp->prev = tail;
		tail = temp;
	}
	printf("Creating & adding process %d to queue with burst time %d\n",thisProcess->p_id,thisProcess->burst_time);
}

void runProcess() {
	
	if (myQueue == NULL) {
		printf("No Process to run.\n");
	}
	else if (myQueue->proc.burst_time == 1) {
		printf("Process %d completed.\n",myQueue->proc.p_id);
		struct qptr temp = myQueue;
		myQueue = myQueue->next;
		free(temp);	
	}
	else
		myQueue->proc.burst_time--;	
}

void * Creator(void *param) {
	int num = atoi(param) ; 
	srand(time(NULL));
	for(int i=1;i<=num;i++){
		pthread_mutex_lock(&mut);
		struct Process newp;
		newp.p_id = i;
		newp.burst_time = (rand()%MAXBURSTTIME)+1;
		createProcess(&newp) ;
		pthread_mutex_unlock(&mut);
		sleep(1);
	}
	
	pthread_exit(0);
}

void * Scheduler() {
	while(1) {
		pthread_mutex_lock(&mut);
		for(int i = 0; i < 5; i++)
			runProcess();
		pthread_mutex_unlock(&mut);
		sleep(1);
	}
	pthread_exit(0);
}

int main() {
	pthread_t cid,sid;
	pthread_mutex_init(&mut,NULL);
	char nump[10] = "10\0";
	pthread_create(&cid , NULL , Creator  , nump);
	pthread_create(&sid , NULL , Scheduler  , NULL);
	
	pthread_join(cid,NULL);
	pthread_join(sid,NULL);
	pthread_mutex_destroy(&mut);
	
	return 0;
}
