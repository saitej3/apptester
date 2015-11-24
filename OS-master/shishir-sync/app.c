#include <stdio.h>
#include "sthread.c"
#include "syn.c"
#define mod 1000000007
struct sthread_mutex T;
int A[10000];
struct limit{
int a,b;};
int global_sum=0;
void fn1(void * arg)
{
	struct limit * L=arg;
	printf("Hello\n");
	int i;
	int sum=0;
	for(i=L->a;i<L->b;i++)
	{
		sum=(sum+A[i])%mod;
	}
	sthread_mutex_lock(&T);
	global_sum+=sum;
	sthread_mutex_unlock(&T);
	printf("now i have slept");
}
int check()
{
	int i,a;
	struct limit L;
	for(i=0;i<10000;i++)
		A[i]=i+1;
	struct sthread_t t,t1,t2,t3;
	sthread_mutex_init(&T);
	printf("Calling thread craetion\n");
	L.a=0;
	L.b=2500;
 	sthread_create(&t,fn1,(void *)(&L));
 	struct limit L1;
 	L1.a=L.b;
 	L1.b=L.b+2500;
	sthread_create(&t1,fn1,(void *)(&L1));
	struct limit L2;
	L2.a=L1.b;
 	L2.b=L1.b+2500;
	sthread_create(&t2,fn1,(void *)(&L2));
	struct limit L3;
	L3.a=L2.b;
 	L3.b=L2.b+2500;
	sthread_create(&t3,fn1,(void *)(&L3));
     sthread_wait(&t);
     sthread_wait(&t1);
     sthread_wait(&t2);
	sthread_wait(&t3);
	printf("Global sum %d",global_sum);
}
