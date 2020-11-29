#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>

#define MAXLINE 80

int main (int argc, char *argv[]) 
{

    int n, cfd;
    struct hostent *h;
    struct sockaddr_in saddr;
    char buf[MAXLINE];
    char *host = argv[1];
    int port = atoi(argv[2]);

    write(1,"Insert your name : ",20);    
    n = read(0,buf,MAXLINE);
    
    if ((cfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket() failed.\n"); 
        exit(1);
    }
    if ((h = gethostbyname(host)) == NULL) {
        printf("invalid hostname %s\n", host); 
        exit(2);
    }   
    memset((char *)&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    memcpy((char *)&saddr.sin_addr.s_addr, (char *)h->h_addr, h->h_length);
    saddr.sin_port = htons(port);

    if (connect(cfd,(struct sockaddr *)&saddr,sizeof(saddr)) < 0) {
        printf("connect() failed.\n");
        exit(3);
    }
    
    write(cfd, buf, n);    

    
    fd_set readset,copyset;
    int fdnum;
    FD_ZERO(&readset);
    FD_SET(0,&readset);
    FD_SET(cfd,&readset);  

    while (1) 
    {
        copyset = readset;

        if((fdnum=select(cfd+1, &copyset, NULL, NULL, NULL)) < 0 ) 
        {
            printf("select() failed.\n");
            exit(4);
        }

        if(FD_ISSET(0,&copyset))
        {        
            n = read(0,buf,MAXLINE);
            if(buf[0]=='q' && buf[1]=='u' && buf[2]=='i'&&buf[3]=='t'&&buf[4]=='\n') break;
            write(cfd,buf,n);
        }

        if(FD_ISSET(cfd,&copyset))
        {            
            n = read(cfd,buf,MAXLINE);
            write(1,buf,n);
        }
    }
    
    close(cfd);

    return 0;
}