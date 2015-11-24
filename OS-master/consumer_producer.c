#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX 10

pthread_mutex_t mutex;
pthread_cond_t cc, cp;
int queue[MAX];
int head, tail, count;

void* producer(void *ptr) {
  int i;

  for (i = 1; i <= MAX; i++) {
    pthread_mutex_lock(&mutex); /* protect buffer */
    
    while (count==MAX)          
      pthread_cond_wait(&cp, &mutex);
    queue[tail] = rand();
    printf("Produced %d!\n",queue[tail]);
    tail = (tail+1)%MAX;
    count++;
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cc); 
  }
  pthread_exit(0);
}

void* consumer(void *ptr) {
  int i;

  for (i = 1; i <= MAX; i++) {
    pthread_mutex_lock(&mutex); /* protect buffer */
    
    while (count == 0)     /* If there is nothing in 
             the buffer then wait */
    	
      pthread_cond_wait(&cc, &mutex);
    printf("Consumed %d!\n",queue[head]);
    head = (head+1)%MAX;
    count--;
    pthread_mutex_unlock(&mutex); /* release the buffer */
    
    pthread_cond_signal(&cp);  /* wake up consumer */
    
  }
  pthread_exit(0);
}

int main(int argc, char **argv) {
  pthread_t pro, con;
  head = tail = count = 0;
  // Initialize the mutex and condition variables
  /* What's the NULL for ??? */
  
  pthread_mutex_init(&mutex, NULL); 
  pthread_cond_init(&cc, NULL);    /* Initialize consumer condition variable */
  
  pthread_cond_init(&cp, NULL);    /* Initialize producer condition variable */
  

  // Create the threads
  pthread_create(&con, NULL, consumer, NULL);
  pthread_create(&pro, NULL, producer, NULL);

  // Wait for the threads to finish
  // Otherwise main might run to the end
  // and kill the entire process when it exits.
  pthread_join(con, NULL);
  pthread_join(pro, NULL);

  // Cleanup -- would happen automatically at end of program
  pthread_mutex_destroy(&mutex);  /* Free up mutex */
  
  pthread_cond_destroy(&cc);   /* Free up consumer condition variable */
  
  pthread_cond_destroy(&cp);   /* Free up producer condition variable */
  

}
