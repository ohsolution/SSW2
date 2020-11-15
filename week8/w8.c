#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARG 128
#define MAX_PATH_LEN 128

void make_tokens(char* cmd, char* arg[], char* arg2[], char* target) {
    char *left, *right, *ptr;
    int num = 0;

    left = strtok(cmd, target);
    strcat(left, "\0");
    right = strtok(NULL, target);
    strcat(right, "\0");

    ptr = strtok(left, " ");
    while (ptr != NULL) {
        arg[num++] = ptr;
        ptr = strtok(NULL, " ");
    }
    arg[num] = NULL;

    num = 0;
    ptr = strtok(right, " ");
    while (ptr != NULL) {
        arg2[num++] = ptr;
        ptr = strtok(NULL, " ");
    }
    arg2[num] = NULL;

    return;
}

int main () {
    size_t size;
    char *cmd, *ptr;
    char *arg[MAX_ARG], *arg2[MAX_ARG];
    char path[MAX_PATH_LEN], path2[MAX_PATH_LEN];
    int child_status;
    int fd[2];
    int fdr;
    
    while(1) 
    {
        printf("> ");

        int num = 0;
        cmd = NULL;
        getline(&cmd, &size, stdin);
        cmd[strlen(cmd)-1] = '\0';        

	// TERMINATE PROGRAM
        if(strcmp(cmd, "quit") == 0) {
            break;
        }

	// PIPE 
        if(strchr(cmd, '|') != NULL) 
        {                
            make_tokens(cmd, arg, arg2, "|");
            sprintf(path, "/bin/%s", arg[0]);
            sprintf(path2, "/bin/%s", arg2[0]);

            if(pipe(fd) == -1) exit(1);
            memset(pipe,-1,sizeof(pipe));

            if(fork() == 0) 
            {                             
                dup2(fd[0],STDIN_FILENO);
                for(int i=0;i<2;++i) if(fd[i]==-1) close(fd[i]);
                execv(path2,arg2);                            
                exit(0);
            }
            else
            {
                close(fd[0]);

                int tpid = fork();        
                if(!tpid)
                {                        
                    dup2(fd[1],STDOUT_FILENO);    
                    execv(path,arg);                                    
                    exit(0);
                }
                else waitpid(tpid,NULL,WUNTRACED);
            }     
            
        } 
	// > REDIRECTION
        else if(strchr(cmd, '>') != NULL) 
        {
            make_tokens(cmd, arg, arg2, ">");
            sprintf(path, "/bin/%s", arg[0]);

            if(fork() == 0) 
            {
                int tfd = open(arg2[0],O_WRONLY | O_CREAT,0644);

                dup2(tfd,STDOUT_FILENO);                
                execv(path, arg);
                exit(0);                
            }
            else wait(&child_status);

        }
	// < REDIRECTION
        else if(strchr(cmd, '<') != NULL) 
        {
            make_tokens(cmd, arg, arg2, "<");
            sprintf(path, "/bin/%s", arg[0]);
            
            int tcnt = 0;
            while(arg[++tcnt]);
            arg[tcnt++] = arg2[0];
            arg[tcnt] = NULL;
            
            if(fork() == 0) 
            {                                                
                execv(path, arg);
                exit(0);                
            }
            else wait(&child_status);

        }

	// ONLY SINGLE COMMAND
        else {
            ptr = strtok(cmd, " ");
            while (ptr != NULL) {
                arg[num++] = ptr;
                ptr = strtok(NULL, " ");
            }
            arg[num] = NULL;

            for(int i=0;arg[i];++i)
            {
                int c =-1;
                while(arg[i][++c] !='\0') printf("%c",arg[i][c]);
                printf("\n");
            }            

            sprintf(path, "/usr/bin/%s", arg[0]);
        
            if (fork() == 0) {
                execv(path, arg);
                exit(0);
            }
            else
                wait(&child_status);            
        }
    }
    return 0;
}   

