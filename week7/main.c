#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>


int send_sig_cnt;
int recv_sig_cnt;
int recv_ack_cnt;
pid_t pid;

int max(int a, int b) {return a>b ? a : b;}

void proc_exit_handler(int sig) //sigint
{
	printf("receiver: recivng signal %d\n",recv_sig_cnt);
	exit(getpid());
}

void recv_sig_handler(int sig)
{
	printf("receiver: receive %d signal and sending acks\n",++recv_sig_cnt);	
	kill(pid,SIGUSR2);

}
void send_sig_handler(int sig)
{
	printf("sender : sending signal: %d\n",max(0,send_sig_cnt-recv_ack_cnt));	

	if(!max(send_sig_cnt-recv_ack_cnt,0))
	{
		printf("all signal are sended\n");
		kill(pid,SIGINT);
		sleep(1);
		exit(getpid());
	}

	for(int idx= 0 ; idx< send_sig_cnt;++idx) kill(pid,SIGUSR1); // send sig usr to recv_ack_handler
	alarm(1);
}

void recv_ack_handler(int sig)
{
	++recv_ack_cnt;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "The number of argument should be 2\n");
		exit(1);
	}

	send_sig_cnt = atoi(argv[1]);

	printf("sending signal: %d\n", send_sig_cnt);

	/* Install Signal Handler */

	if((pid = fork())==0)
	{
		signal(SIGUSR1,recv_sig_handler);
		signal(SIGINT,proc_exit_handler);
		pid = getppid();
		while(1);
	}
	else
	{
		signal(SIGALRM,send_sig_handler);
		signal(SIGUSR2,recv_ack_handler);		
		alarm(1);
		while(1);
	}
	
	
	return 0;
}
