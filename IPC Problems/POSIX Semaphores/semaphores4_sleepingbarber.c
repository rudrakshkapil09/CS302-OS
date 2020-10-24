#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_CUSTOMERS 50

// Function prototypes:
void *customer(void *num);
void *barber(void *);
void randwait(int secs);

// Define the semaphores:
sem_t waitingRoom;	//number of customers in waiting room
sem_t barberChair;	//mutex for chair
sem_t barberPillow;	//allows barber to sleep till customer arrives
sem_t seatBelt;		//keeps customer in barber's chair

//flag to check if all customers serviced
int allDone = 0;

//MAIN
int main(int argc, char *argv[]) {
    pthread_t btid;
    pthread_t tid[MAX_CUSTOMERS];

    int i, numCustomers, numChairs;

    printf("Enter the number of Customers : "); 
	scanf("%d",&numCustomers) ;
    printf("Enter the number of Chairs In Waiting Room : "); 
	scanf("%d",&numChairs);

    // Initialize the semaphores
    sem_init(&waitingRoom, 0, numChairs);
    sem_init(&barberChair, 0, 1);
    sem_init(&barberPillow, 0, 0);
    sem_init(&seatBelt, 0, 0);

    // Create the barber thread
    pthread_create(&btid, NULL, barber, NULL);

    // Create the customer threads
    for (i=0; i<numCustomers; i++) {
        pthread_create(&tid[i], NULL, customer, (void *)i);
        sleep(1);
    }

    // wait for them to finish.
    for (i = 0; i < numCustomers; i++) {
        pthread_join(tid[i],NULL);
        sleep(1);
    }

	// When all of the customers are finished, kill the barber thread.
    allDone = 1;
    sem_post(&barberPillow); // Wake the barber so he will exit.
    pthread_join(btid,NULL);
}

void *customer(void *number) {
    printf("Customer %d arrived at barber shop.\n", number);

    sem_wait(&waitingRoom);									// wait for waiting room spot
    printf("Customer %d entered waiting room.\n", number);
    sem_wait(&barberChair);									// wait for chair to be empty
	printf("Customer %d sat in barber's chair.\n", number);
    sem_post(&waitingRoom);   								// signal free spot in waiting room.

    sem_post(&barberPillow);								// signal to wake up the barber
    printf("Customer %d woke the barber.\n", number);

    sem_wait(&seatBelt);									// Wait for the barber to finish cutting your hair.
	printf("Customer %d done with haircut.\n", number);

    sem_post(&barberChair);									// signal that barber chair is empty
    printf("Customer %d leaving barber shop.\n", number);
}

void *barber(void *junk) {
	//while customers are still there
    while (!allDone) {

        printf("The barber is sleeping.\n");					// Sleep until someone arrives and wakes you
        sem_wait(&barberPillow);
		printf("The barber is awake.\n");

        if (!allDone) {
			//cut hair for some amount of time
            printf("The barber is cutting hair\n");
            sleep(2);
            printf("The barber has finished cutting hair.\n");

            sem_post(&seatBelt);            					// Release the customer when done cutting
        }
        else printf("The barber is going home for the day.\n");
    }
}
