#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#define MAX_THREAD 20
const int MEM = 64000;

struct rthread_t{
        int r_id;
        int r_size;
        ucontext_t *thread;
};

struct queue{
        struct rthread_t * task;
        struct queue * next;
};

struct queue * head,*tail;
struct rthread_t *currrent_thread;
struct sigaction action;
struct itimerval it;

int id,count;
ucontext_t Main, last;

void kill_thread(){
        ucontext_t temp;
        if(head->next == NULL){
                currrent_thread = NULL;
                it.it_interval.tv_sec = 0;
                it.it_interval.tv_usec = 0;
                it.it_value.tv_sec = 0;
                it.it_value.tv_usec = 0;
                setitimer(ITIMER_PROF,&it,NULL);
                printf("Last thread killed!\n");
                head = NULL;
                setcontext(&Main);
        }else{
                currrent_thread = head->next->task;
                head=head->next;
                swapcontext(&temp,currrent_thread->thread);
        }
}


void rthread_init(struct rthread_t  *new_task){
        new_task->thread = (ucontext_t *)malloc(sizeof(ucontext_t));
        getcontext(new_task->thread);
        getcontext(&last);
        last.uc_stack.ss_sp = malloc(MEM);
        last.uc_stack.ss_size = MEM;
        last.uc_stack.ss_flags = 0;
        makecontext(&last,kill_thread, 0);
        new_task->thread->uc_link = &last;
        new_task->thread->uc_stack.ss_sp = malloc(MEM);
        new_task->thread->uc_stack.ss_size = MEM;
        new_task->thread->uc_stack.ss_flags = 0;
        new_task->r_size = MEM;
        new_task->r_id = id++;
}

void block(){
        sigfillset(&action.sa_mask);
        sigprocmask(SIG_BLOCK,&action.sa_mask,NULL);
}
void unblock(){
        sigprocmask(SIG_UNBLOCK,&action.sa_mask,NULL);
        sigemptyset(&action.sa_mask);
}

d

void schedule(){
        printf("Scehduling thread\n");
        if(currrent_thread==NULL){
                currrent_thread = head->task;
                swapcontext(&Main,currrent_thread->thread);
        }else{
                if(head->next==NULL){
                        it.it_interval.tv_sec = 0;
                        it.it_interval.tv_usec = 0;
                        it.it_value.tv_sec = 0;
                        it.it_value.tv_usec = 0;
                        setitimer(ITIMER_PROF, &it, NULL);
                        setcontext(currrent_thread->thread);
                }else{
                        currrent_thread = head->next->task;
                        enqueue(head->task);
                        struct queue* temp = head;
                        head = head->next;
                        swapcontext(tail->task->thread,currrent_thread->thread);
                }
        }
}


void init(struct itimerval it){
         struct sigaction controller, oldcontroller;
         controller.sa_handler = schedule;
         sigemptyset(&controller.sa_mask);
         controller.sa_flags = 0;
         sigaction(SIGPROF, &controller, &oldcontroller);
         it.it_interval.tv_sec = 0;
         it.it_interval.tv_usec = 1000;
         it.it_value.tv_sec = 0;
         it.it_value.tv_usec = 5000;
         setitimer(ITIMER_PROF, &it, NULL);
}

int enqueue(struct rthread_t * new_task){
        printf("Inserting in thread queue\n");
        printf("Current queue size: %d\n",count);
        block();
        if(head==NULL)
        {
                getcontext(&Main);
                init(it);
                head = (struct queue *)malloc(sizeof(struct queue) );
                tail = head;
                head->next = tail->next = NULL;
                head->task = new_task;
                count++;
                unblock();
                return 1;
        }else{
                if(count >= MAX_THREAD){
                        int i;
                        printf("Stack overflow!.... Stopping program execution\n");
                        scanf("%d",&i);
                        unblock();
                        return -1;
                }else{
                        struct queue *temp = (struct queue *)malloc(sizeof(struct queue));
                        temp->next = NULL;
                        temp->task = new_task;
                        count++;
                        tail->next = temp;
                        tail = temp;
                        unblock();
                        return 0;
                }
        }
}


int rthread_create(struct rthread_t *new, void (* fn)(), void *arg){
        printf("Request recieved\n");
        rthread_init(new);
        printf("Thread %d initialized\n", new->r_id);
        makecontext(new->thread,fn,0,arg);
        printf("Function associated\n");
        return enqueue(new);
}

const int T = 1000;

void fnc(void){
        int t = T;
        while( t-- ){
                printf("FUNCTION 1 %d\n",t);
        }
}

void fnc2(void){
        int t = T;
        while( t-- ){
                printf("\t\t\t\tFUNCTION 2 %d\n",t);
        }
}

void fnc3(void){

}

int rthread_mutex_lock(){

}

int rthread_mutex_unlock(){

}

int rthread_mutex_init(){

}

int rthread_cond_init(){

}

int rthread_cond_signal(){

}

int rthread_cond_wait(){

}

int main(){
        struct rthread_t s1, s2;
        id = count = 0;
        printf("Thread %d creation flag: %d\n",s1.r_id,rthread_create(&s1,(void*)(*fnc),NULL));
        printf("Thread %d creation flag: %d\n",s2.r_id,rthread_create(&s2,(void*)(*fnc2),NULL));
        int status = 1;
        do{
                block();
                status = (head==NULL ? 0 : 1);
                unblock();
        }while( status );
        printf("All threads finished execution\n");
}

