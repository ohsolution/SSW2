#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>

char rm[20] = "read message at:\t";

struct msg
{
	long msgtype;
	char text[512];
};

struct msg_ack 
{
	long msgtype;
	char text[512];
};

void mattach(char ori[],long id)
{
	int idx=0;
	int c=0;
	char tmp[512];

	while(id)
	{
		tmp[c++] = (id%10)+'0';
		id/=10;
	}

	for(int i= c-1;i>-1;--i) ori[idx++] = tmp[i];

	ori[idx++] = ' ';
	for(int i=0;i<20,rm[i]!='\0';++i) ori[idx++] = rm[i];

	time_t ct;
	time(&ct);
	char * tstr = ctime(&ct);
	//printf("%s\n",tstr);
	for(int i=0;tstr[i]!='\0';++i) ori[idx++] = tstr[i];
	return;
}

int isquit(char str[])
{
	return (str[0]== 'q' && str[1]=='u' && str[2]=='i'&&str[3]=='t'&&str[4]=='\n'&&str[5]=='\0');
}

int main()
{
	/* 
	 * @ int uid: user ID
	 * @ int receiver_id: receiver's ID
	 */ 
	int uid; 
	int receiver_id;
	

	printf("my id is \n");
	scanf("%d", &uid);
	getchar(); // flush

	printf("who to send ? \n");
	scanf("%d", &receiver_id); 
	getchar(); // flush  

	int rack_id = uid + uid + receiver_id;
	int uack_id = receiver_id + receiver_id + uid;

	key_t key = ftok(".",'a');
	int mqid = msgget(key,IPC_CREAT | 0660);	
	pid_t pid = fork();

	if (!pid) 
	{
		while (1) 
		{			
			/* receive message */ 
			/* get msg from queue and print out */ 
			struct msg recivemessage;
			struct msg ackmessage;

			//ssize_t sz = msgrcv(mqid,(void *)recivemessage,sizeof(struct msg),receiver_id,IPC_NOWAIT);
						
			if(msgrcv(mqid,(void *)(&recivemessage),sizeof(struct msg),receiver_id,IPC_NOWAIT) > 0)
			{				
				printf("RECEIVED %s\n",recivemessage.text);
				
				struct msg_ack ackmessage;				
				mattach(ackmessage.text,uid);
				ackmessage.msgtype = uack_id;				
				msgsnd(mqid,(void *)(&ackmessage),sizeof(struct msg_ack),0);
			}		

			if(msgrcv(mqid,(void *)(&ackmessage),sizeof(struct msg),rack_id,IPC_NOWAIT) > 0) printf("%s\n",ackmessage.text);			
			
		}		
	} 
	else
	{		
		while (1) 
		{
			/* send message */ 
			/* get user input and send to the msg queue */

			struct msg sendmassage;

			sendmassage.msgtype = uid;
			size_t len = 0;			
			char * buf=NULL;
			getline(&buf,&len,stdin);

			int c= 0;
			for(int i=0;buf[i]!='\0';++i) sendmassage.text[c++]=buf[i];
			sendmassage.text[c]='\0';
			
			if(isquit(sendmassage.text))
			{			
				kill(pid,SIGKILL);
				exit(getpid());
			}

			free(buf);
			msgsnd(mqid,(void *)(&sendmassage),sizeof(struct msg),0);									
		}
	}	

	return 0;
}
