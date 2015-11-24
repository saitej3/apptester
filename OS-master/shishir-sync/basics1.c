#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>
#define MEM 64000
ucontext_t T1,T2,Main;
ucontext_t a;
int fn1()
{
	printf("this is from 1\n");
	setcontext(&Main);
}
void sighandler(int sig)
{
 
 printf("signal occurred times\n");
}
void fn2()
{
	printf("This is from 2\n");
	setcontext(&a);
	printf("Finished\n");
}
void init(){
	struct itimerval it;
 struct sigaction act, oact;
 act.sa_handler = sighandler;
 sigemptyset(&act.sa_mask);
 act.sa_flags = 0;

 sigaction(SIGPROF, &act, &oact); 
 // Start itimer
 it.it_interval.tv_sec = 4;
 it.it_interval.tv_usec = 50000;
 it.it_value.tv_sec = 1;
 it.it_value.tv_usec = 100000;
 setitimer(ITIMER_PROF, &it, NULL);
}
void start()
{
	init();
	while(1){
		
	}
	getcontext(&a);
	a.uc_link=0;
	a.uc_stack.ss_sp=malloc(MEM);
	a.uc_stack.ss_size=MEM;
	a.uc_stack.ss_flags=0;
	makecontext(&a,(void *)&fn1,0);
}
int main()
{

	start();
	getcontext(&Main);
	getcontext(&T1);
	T1.uc_link=0;
	T1.uc_stack.ss_sp=malloc(MEM);
	T1.uc_stack.ss_size=MEM;
	T1.uc_stack.ss_flags=0;
	makecontext(&T1,(void *)&fn1,0);
	swapcontext(&Main,&T1);
	getcontext(&T2);
	T2.uc_link=0;
	T2.uc_stack.ss_sp=malloc(MEM);
	T2.uc_stack.ss_size=MEM;
	T2.uc_stack.ss_flags=0;
	makecontext(&T2,(void *)&fn2,0);
	swapcontext(&Main,&T2);
	printf("completed\n");
	exit(0);
}