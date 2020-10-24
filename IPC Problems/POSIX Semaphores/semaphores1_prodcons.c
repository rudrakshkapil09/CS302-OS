#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFF_SIZE   5           /* total number of slots */
#define NP          3           /* total number of producers */
#define NC          3           /* total number of consumers */
#define NITERS      40           /* number of items produced/consumed */

typedef struct
{
    int buf[BUFF_SIZE];   /* shared var */
    int in;               /* buf[in%BUFF_SIZE] is the first empty slot */
    int out;              /* buf[out%BUFF_SIZE] is the first full slot */
    sem_t full;           /* keep track of the number of full spots */
    sem_t empty;          /* keep track of the number of empty spots */
    sem_t mutex;          /* enforce mutual exclusion to shared data */
} sbuf_t;

sbuf_t shared;

void *Producer(void *arg)
{
    int i, item, index;
    index = (int)arg;
    
    for (i = 0; i < NITERS; i++) {
        item = i;

        sem_wait(&shared.empty);
        sem_wait(&shared.mutex);
        
        /*Critical Section*/
        printf("[P%d] Producing at %d ...\n", index, shared.in);
        shared.buf[shared.in] = item;
        shared.in = (shared.in+1)%BUFF_SIZE;

        sem_post(&shared.mutex);
        sem_post(&shared.full);

        //Interleave  producer and consumer execution 
        if (i % 2 == 1)
            sleep(1);
    }
}

void *Consumer(void *arg)
{
    int i, item, index;

    index = (int)arg;
    for (i = NITERS; i > 0; i--) {
        sem_wait(&shared.full);
        sem_wait(&shared.mutex);
        
        item=shared.buf[shared.out];
        printf("[C%d] Consuming at    %d ...\n", index, shared.out);
        shared.out = (shared.out+1)%BUFF_SIZE;
        
        sem_post(&shared.mutex);
        sem_post(&shared.empty);

        //Interleave  producer and consumer execution
        if (i % 2 == 1)
            sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_t idP, idC;
    int index;

    sem_init(&shared.full, 0, 0);
    sem_init(&shared.empty, 0, BUFF_SIZE);
    sem_init(&shared.mutex, 0, 1);

    //create producers and consumers
    for (index = 0; index < NP; index++)
        pthread_create(&idP, NULL, Producer, (void*)index);

    for (index = 0; index < NC; index++)
        pthread_create(&idC, NULL, Consumer, (void*)index);

    pthread_exit(NULL);
}
