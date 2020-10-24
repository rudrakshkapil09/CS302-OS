//Reader Favoring Solution
#include<pthread.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

pthread_mutex_t x,wsem;
int readcount = 0;
int sh_var = 5;

void * reader (void *param) {

    printf("\nReader-%d is trying to enter", param);    
    pthread_mutex_lock(&x);
    readcount++;
    if (readcount == 1)
        pthread_mutex_lock(&wsem);
    pthread_mutex_unlock(&x);
	printf("\nReader-%d is has entered", param);   

    printf("\n%d reader(s) inside", readcount);
	printf("\nCurrent value => %d\n", sh_var); 
    sleep(1);
    
    printf("\nReader-%d is trying to leave", param);    
    pthread_mutex_lock(&x);
    readcount--;
    if (readcount == 0)
        pthread_mutex_unlock(&wsem);
    pthread_mutex_unlock(&x);
    printf("\nReader-%d has left", param);
}

void * writer (void * param) {

    printf("\nWriter-%d is trying to enter", param);
    pthread_mutex_lock(&wsem);
    printf("\nWriter-%d has entered", param);

	sh_var+=5;
	printf("\nUpdated value (+5) => %d\n", sh_var); 
    sleep(1);

    printf("\nWriter-%d is trying to leave", param);    
    pthread_mutex_unlock(&wsem);
    printf("\nWriter-%d is leaving", param);
}

int main() {
	printf("\nInitial Value => 5\n");
	pthread_t read[5], write[5];
	pthread_mutex_init(&x,NULL);
    pthread_mutex_init(&wsem,NULL);

	for (int i = 0; i < 5; i++) {
		pthread_create(&read[i], NULL, reader, (void *)i);
		pthread_create(&write[i], NULL, writer, (void *)i);
	}
	
	for (int i = 0; i < 5; i++) {
		pthread_join(read[i], NULL);
		pthread_join(write[i], NULL);
	}
	printf("\n\nFinal Value => %d", sh_var);
    return 0;
}
