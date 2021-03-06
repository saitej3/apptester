
#define NTHREADS 5
extern void chopsticks_init();
extern void chopsticks_take(int i);
extern void chopsticks_put(int i);
extern void chopsticks_finalize();
extern void chopsticks_emergency_stop();
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#include "chopsticks.h"

#define LEFT (i+NTHREADS-1)%NTHREADS /* philo to the left */
#define RIGHT (i+1)%NTHREADS         /* philo to the right */
#define THINKING 1                   /* assign values to states */
#define HUNGRY 2
#define EATING 3


pthread_cond_t  CV[NTHREADS];    /* one per philosopher */
pthread_mutex_t M;               /* mutual exclusion for the monitor */
int             state[NTHREADS]; /* state of each philosopher */


int 
update_state (int i) {
  if (state[i] == HUNGRY
   && state[LEFT] != EATING
   && state[RIGHT] != EATING) {
     state[i] = EATING;
     pthread_cond_signal (&CV[i]);
  }
  return 0;
}


void
chopsticks_init () {
  int i;
  pthread_mutex_init (&M, NULL);
  for (i = 0; i < NTHREADS; i++) {
    pthread_cond_init (&CV[i], NULL);
    state[i] = THINKING;
  }
}

void
chopsticks_take (int i) {
   pthread_mutex_lock (&M);      /* enter cs, lock mutex */
   state[i] = HUNGRY;            /* set philosopher's state to HUNGRY */
   update_state(i);              /* update_state philosopher */
   while (state[i] == HUNGRY)    /* loop while philosopher is hungry */
     pthread_cond_wait (&CV[i],&M);
   pthread_mutex_unlock (&M); /* exit cs, unlock mutex */
}

void
chopsticks_put (int i) {
  pthread_mutex_lock (&M);       /* enter cs, lock mutex */
  state[i] = THINKING;
  update_state (LEFT);           /* update_state neighbors */
  update_state (RIGHT);
  pthread_mutex_unlock (&M);     /* exit cs, unlock mutex */
}

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

pthread_mutex_t outlock;   /* protects against output interleaving */
int nsteps, maxsteps = 0;  /* number of steps to run this test */

int eat_count[NTHREADS];          /* number of steps for each thread */

void trace(int i, char *s) {
  /* print out message, for use in execution tracing
     i = philospher ID
     s = message 
   */ 
  pthread_mutex_lock(&outlock);
  if (strcmp (s, "eating") == 0) eat_count [i]++;
/*
  fprintf(stdout,"%d: %s\n",i,s);
 */
  if (nsteps++ > maxsteps) {
    /* don't exit while we are holding any chopsticks */
    if (strcmp(s,"thinking") == 0) {
      pthread_mutex_unlock(&outlock); 
/*
      fprintf (stderr, "thread done\n");
 */
      pthread_exit(0);
    }
  }
  pthread_mutex_unlock(&outlock);
}

void * philosopher_body(void *arg) {
  int self = *(int *) arg;
  for (;;) {
     trace(self,"thinking");
     chopsticks_take(self);
     trace(self,"eating");
     chopsticks_put(self);
  }
}

int main() {
  int i; 
  pthread_t th[NTHREADS]; /* IDs of the philospher threads */
  int       no[NTHREADS]; /* corresponding table position numbers*/
  pthread_attr_t attr;

  for (i = 0; i < NTHREADS; i++) eat_count [i] = 0;

  pthread_mutex_init(&outlock, NULL);

  chopsticks_init();

  fprintf(stdout,"enter number of steps to run: "); fflush(stdout);
  fscanf(stdin,"%d",&maxsteps);

  pthread_attr_init (&attr); /* sets default attributes */

  pthread_setconcurrency (4);

  pthread_attr_setscope (&attr, PTHREAD_SCOPE_SYSTEM);

  for (i = 0; i < NTHREADS; i++) {
    no[i] = i;
    pthread_create(&th[i], NULL, philosopher_body, (int *) &no[i]);
  }

  for (i = 0; i < NTHREADS; i++) pthread_join(th[i], NULL);

  for (i = 0; i < NTHREADS; i++) {
     fprintf (stderr, "philospher %d ate %d times\n", i, eat_count [i]);
  }

  return 0;
}


