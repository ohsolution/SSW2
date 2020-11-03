#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

const unsigned int min = 60;
char pad[2] = " ";

int execute(FILE * fp)
{
	char * cmd = NULL;
	size_t size;
	int c = 0;

	if(getline(&cmd,&size,fp)<=1) return 0;

	char * ptr = cmd;
	int sz = strlen(cmd)-1;

	cmd[sz] = '\0';	
	char ** args = (char**)malloc(sizeof(char*)*sz);
	char * str = malloc(sizeof(char)* sz);
	str[0]= '\0';

	char * pstr = str;
	char * pch = strtok(cmd," ");

	time_t rt;
	struct tm* tinfo;
	time(&rt);
	tinfo = localtime(&rt);
	
	if((pch[0] != '*') && (atoi(pch)!=tinfo->tm_min)) goto EXIT;
	pch = strtok(NULL," ");
	if(pch[0] != '*' && atoi(pch)!=tinfo->tm_hour) goto EXIT;
	pch = strtok(NULL," ");

	while(pch)
	{		
		args[c++] = pch;		
		pch = strtok(NULL," ");
	}
	args[c] = NULL;
	
	for(int i=0;i<c;++i) 
	{
		strcat(str,args[i]);
		strcat(str,pad);
	}
	
	if(fork()) waitpid(-1,NULL,WNOHANG);
	else
	{
		execl("/bin/bash","/bin/bash","-c",str,NULL);
		exit(0);
	}
	
	EXIT:
	free(args);
	free(ptr);
	free(str);
	return 1;
}

int main(void)
{
	int pid;
	int fdcron, fdin, fdout, fderr;
	
	if((pid = fork()) < 0)
		exit(1);
	
	if(pid != 0)
		exit(0);
	
	if(setsid() < 0 || chdir("/") < 0)
		exit(1);

	umask(0);
	
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	
	fdin = open("/dev/null", O_RDWR);
	fdout = open("/dev/null", O_RDWR);
	fderr = open("/dev/null", O_RDWR);

	chdir("/");
	struct tm* localtime(const time_t * timer);

	while (1) 
	{
		FILE * fp = fopen("/etc/simplecrontab","r");
		
		while(execute(fp));

		sleep(min);
		fclose(fp);
	}

	return 0;
}
