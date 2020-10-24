#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#define MAXCHAIRS 5

sem_t customer,barber;
pthread_mutex_t seats;
int freeSeats = MAXCHAIRS;

void * Barber()
{
	while (1) {
        printf("Barber sleeping while waiting\n");
		sem_wait(&customer);
		printf("Barber awake\n");
        
        pthread_mutex_lock(&seats);
        freeSeats++;
        pthread_mutex_unlock(&seats);
        
        printf("Barber sits customer down\n");
		sem_post(&barber);
		printf("Barber is cutting hair\n");
		sleep(2);
    }
}

void * Customer()
{
    while (1) {
        pthread_mutex_lock(&seats);         //get lock
        if (freeSeats > 0) {
            printf("Sitting down\n");
            freeSeats--;
            printf("Notify barber\n");
            sem_post(&customer);
            pthread_mutex_unlock(&seats);   //release lock
            sem_wait(&barber);
            printf("Customer leaving\n");
        }
        else  pthread_mutex_unlock(&seats);
    }
}

int main()
{
	pthread_t thread[40];
    
	sem_init(&customer,0,0);
	sem_init(&barber,0,0);
    
    //craete barber thread
	pthread_create(&thread[0],NULL,Barber,NULL);
    
    //create customers
	for (int i=1;i<40;i++)
		pthread_create(&thread[i],NULL,Customer,NULL);
    
    //wait for customers threads
	for(int i=1;i<40;i++)
		pthread_join(thread[i],NULL);

    //wait for barber thread
    printf("Barber done for the day\n");
    pthread_join(thread[0],NULL);
}
