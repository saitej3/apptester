#include <stdio.h>
#include <stdlib.h>
#include <linux/ipc.h>
#include <linux/msg.h>

/* Redefines the struct msgbuf */
typedef struct mymsgbuf
{
  long mtype;
  int int_num;
  float float_num;
  char ch;
} mess_t;

int main()
{
  int qid;
  key_t msgkey;

  mess_t sent;
  mess_t received;

  int length;

  /* Initializes the seed of the pseudo-random number generator */
  srand (time (0));

  /* Length of the message */
  length = sizeof(mess_t) - sizeof(long);

  msgkey = ftok(".",'m');

  /* Creates the queue*/
  qid = msgget(msgkey, IPC_CREAT | 0660);

  printf("QID = %d\n", qid);

  /* Builds a message */
  sent.mtype = 1;
  sent.int_num = rand();
  sent.float_num = (float)(rand())/3;
  sent.ch = 'f';

  /* Sends the message */
  msgsnd(qid, &sent, length, 0);
  printf("MESSAGE SENT...\n");

  /* Receives the message */
  msgrcv(qid, &received, length, sent.mtype, 0);
  printf("MESSAGE RECEIVED...\n");

  /* Controls that received and sent messages are equal */
  printf("Integer number = %d (sent %d) -- ", received.int_num,
         sent.int_num);
  if(received.int_num == sent.int_num) printf(" OK\n");
  else printf("ERROR\n");

  printf("Float numero = %f (sent %f) -- ", received.float_num,
         sent.float_num);
  if(received.float_num == sent.float_num) printf(" OK\n");
  else printf("ERROR\n");

  printf("Char = %c (sent %c) -- ", received.ch, sent.ch);
  if(received.ch == sent.ch) printf(" OK\n");
  else printf("ERROR\n");

  /* Destroys the queue */
  msgctl(qid, IPC_RMID, 0);
}