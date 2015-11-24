#ifndef SYNC_C
#define SYN_C
#include "sthread.c"
struct sthread_mutex{
	int lock;
	int owner;
	struct thread_status * head,* tail;
};


void sthread_mutex_init(struct sthread_mutex * mutex)
{
	mutex->lock=0;
	mutex->owner=-1;
}

int add_to_waiting(struct sthread_mutex * mutex,int s_id)
{
	if(mutex->head==NULL)
	{
		mutex->head=(struct thread_status *)malloc(sizeof(struct thread_status));
		mutex->tail=mutex->head;
		mutex->head->s_id=s_id;
		mutex->head->next=NULL;
		mutex->tail->next=NULL;
	}
	else
	{
		struct thread_status * temp=(struct thread_status *)malloc(sizeof(struct thread_status));
		temp->next=NULL;
		temp->s_id=s_id;
		mutex->tail->next=temp;
		mutex->tail=temp;
	}
}


int sthread_mutex_lock(struct sthread_mutex * mutex)
{
	printf("%d attempting to get lock \n",sthread_self());
		block();
	if(mutex->lock==0)
	{
		mutex->lock=1;
		mutex->owner=sthread_self();
		mutex->head=(struct thread_status *)malloc(sizeof(struct thread_status));
		mutex->tail=mutex->head;
		mutex->head->s_id=sthread_self();
		mutex->head->next=NULL;
		mutex->tail->next=NULL;
		unblock();
		return 0;
	}
	else if(mutex->owner==sthread_self())
	{
		unblock();
		return -1;
	}
	else
	{
		printf("lock not available\n");
		add_to_waiting(mutex,sthread_self());
		while(1)
		{
			if(mutex->owner!=sthread_self())
			{
				schedule(0);
				block();
			}
			else
			{
				break;
			}
		}
		unblock();
	}
}


void sthread_mutex_unlock(struct sthread_mutex * mutex)
{
	block();
	if(mutex->owner==sthread_self())
	{
		mutex->lock=0;
		if(mutex->head==NULL)
		{
			unblock();
			return 0;
		}
		else
		{
			struct thread_status * temp=mutex->head;
			mutex->head=mutex->head->next;
			free(temp);
			if(mutex->head==NULL)
			{
				unblock();
				return 0;
			}
			else
			{
				mutex->lock=1;
				mutex->owner=mutex->head->s_id;
				unblock();
				return 0;
			}
		}
	}
	else
	{
		unblock();
		return -1;
	}
}
#endif
