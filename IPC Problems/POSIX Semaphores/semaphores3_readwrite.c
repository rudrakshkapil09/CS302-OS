//writer favouring
#include<semaphore.h>
#include<stdio.h>
#include<pthread.h>

void *reader(void *);
void *writer(void *);

int readcount = 0, writecount = 0, sh_var=5, bsize[5];
sem_t readMutex, writeMutex, readTry, resource;
pthread_t r[5],w[2];

void *reader(void *i)
{
        printf("\n\nReader-%d is reading...", i);

	//entry:
        sem_wait(&readTry);
        sem_wait(&readMutex);
        readcount++;
        if(readcount == 1)		//first reader
            sem_wait(&resource);
        sem_post(&readMutex);
        sem_post(&readTry);
    
	//CS:
        printf("\nCurrent value: %d\n", sh_var);
    
	//exit:
        sem_wait(&readMutex);
        readcount--;
        if(readcount==0)		//last reader
            sem_post(&resource);
        sem_post(&readMutex);
}

void *writer(void *i)
{
        printf("\n\nWriter-%d is writing... (+5)", i);

		//entry:
        sem_wait(&writeMutex);
        writecount++;
        if(writecount == 1)	//first writer locks out readers
            sem_wait(&readTry);	
        sem_post(&writeMutex);

		//CS:
        sem_wait(&resource);	//lock out other writers
        sh_var += 5;
        sem_post(&resource);

        sem_wait(&writeMutex);
        writecount--;
        if (writecount == 0)	//last writer unlocks readers
            sem_post(&readTry);
        sem_post(&writeMutex);
}

int main()
{
	printf("\nInitial Value => %d", sh_var); 
        sem_init(&readMutex,0,1);
        sem_init(&resource,0,1);
        sem_init(&writeMutex,0,1);
        sem_init(&readTry,0,1);
	        
    	//create threads
        pthread_create(&r[0],NULL,(void *)reader,(void *)0);
        pthread_create(&w[0],NULL,(void *)writer,(void *)0);
        pthread_create(&r[1],NULL,(void *)reader,(void *)1);
        pthread_create(&r[2],NULL,(void *)reader,(void *)2);
        pthread_create(&r[3],NULL,(void *)reader,(void *)3);
        pthread_create(&w[1],NULL,(void *)writer,(void *)3);
        pthread_create(&r[4],NULL,(void *)reader,(void *)4);
    
	//wait for them to finish
        pthread_join(r[0],NULL);
        pthread_join(w[0],NULL);
        pthread_join(r[1],NULL);
        pthread_join(r[2],NULL);
        pthread_join(r[3],NULL);
        pthread_join(w[1],NULL);
        pthread_join(r[4],NULL);

        return(0);
}
