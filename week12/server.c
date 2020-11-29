#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
//#include "cvector.h"

#define MAXLINE 80
#define USERNUM 10
#define USERNAME 17

int fdmap[USERNUM];
int vc[USERNUM+7];
char vs[USERNUM+7][USERNAME];

int f(int idx)
{
    for(int i=0;i<USERNUM;++i)
    {
        if(fdmap[i] == idx) return i;
    }

    return -1;
}

int main (int argc, char *argv[]) {

    int n, listenfd, connfd, caddrlen;
    struct hostent *h;
    struct sockaddr_in saddr, caddr;
    char buf[MAXLINE];
    int port = atoi(argv[1]);

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket() failed.\n");
        exit(1);
    }

    memset((char *)&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(port);

    if (bind(listenfd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
        printf("bind() failed.\n");
        exit(2);
    }

    if (listen(listenfd, 5) < 0) {
        printf("listen() failed.\n");
        exit(3);
    }

    fd_set readset, copyset;
    FD_ZERO(&readset);  /* initialize socket set */
    FD_SET(listenfd, &readset);
    int fdmax = listenfd, fdnum;
    int count = 0;
    memset(fdmap,-1,sizeof(fdmap));

    while (1) 
    {
        copyset = readset;

        if((fdnum = select(fdmax + 1, &copyset, NULL, NULL, NULL)) < 0 ) {
            printf("select() failed.\n");
            exit(4);
        }

        for (int i = 0; i < fdmax + 1; i++) {

            if (FD_ISSET(i, &copyset)) {

                if (i == listenfd) {
                    
                    caddrlen = sizeof(caddr);
                    if ((connfd = accept(listenfd, (struct sockaddr *)&caddr, (socklen_t *)&caddrlen)) < 0) {
                        printf ("accept() failed.\n");
                        continue;
                    }

                    //printf("%d\n",connfd);

                    FD_SET(connfd, &readset);

                    for(int j=0;j<10;++j)
                    {
                        if(fdmap[j]==-1)
                        {
                            fdmap[j] = connfd;
                            break;
                        }
                    }

                    if (fdmax < connfd)
                        fdmax = connfd;

                    ++count;                
                }

                else 
                {
                    char message[MAXLINE];
                    int o = 0;

                    if ((n = read(i, buf, MAXLINE)) > 0) 
                    {                        
                        if(!vc[f(i)])
                        {
                            vc[f(i)] = 1;
                            sprintf(vs[f(i)],"%s",buf);
                            vs[f(i)][n-1]='\0';
                            sprintf(message,"%s joined. %d current users\n",vs[f(i)],count);                            
                        }
                        else 
                        {
                            buf[n]='\0';
                            sprintf(message,"%s:%s",vs[f(i)],buf);
                            char tmp[MAXLINE];
                            printf("got %d bytes from user %s.\n",n,vs[f(i)]);                            
                        }

                        while(message[++o]);
                        message[o] = '\n';

                        for(int j=0;j<fdmax+1;++j)
                        {
                            if(i==j || f(j)==-1) continue;
                            write(j,message,o);
                        }
                        
                    }
                    else 
                    {
                        FD_CLR(i, &readset);                        
                        vc[f(i)]=0;                        
                        sprintf(message,"%s leaved. %d current users.\n",vs[f(i)],--count);
                        while(message[++o]);
                        
                        for(int j=0;j<fdmax+1;++j)
                        {
                            if(i==j|| f(j)==-1) continue;
                            write(j,message,o);
                        }

                        fdmap[f(i)] = -1;
                        close(i);
                    }

                }

            }
           
        }

    }

    return 0;

}

