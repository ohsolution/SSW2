#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAXLINE 200107
#define FMAX 57

int main (int argc, char *argv[]) 
{
    int n, cfd;
    struct hostent *h;
    struct sockaddr_in saddr;
    char buf[MAXLINE];

    char *host = argv[1];
    int port = atoi(argv[2]);

    if ((cfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        printf("socket() failed.\n");
        exit(1);
    }

    if ((h = gethostbyname(host)) == NULL) 
    {
        printf("invalid hostname %s\n", host);
        exit(2);
    }
        
    memset((char *)&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    memcpy((char *)&saddr.sin_addr.s_addr, (char *)h->h_addr, h->h_length);
    saddr.sin_port = htons(port);

    if (connect(cfd,(struct sockaddr *)&saddr,sizeof(saddr)) < 0) 
    {
        printf("connect() failed.\n");
        exit(3);
    }

    while ((n = read(0, buf, FMAX)) > 0) 
    {
        if(buf[0]=='q' && buf[1]=='u' && buf[2]=='i' && buf[3]=='t' && buf[4]=='\n')
        {
            write(cfd,buf,n);
            close(cfd);
            return 0;
        }

        int c= -1;
        while(buf[++c]!='\n');
        buf[c] = '\0';

        int fd = open(buf,O_RDONLY);

        buf[c++] = '>';
        int d = c;


        char tbuf[1024];
        int bl=0;

        do
        {
            bl = read(fd,tbuf,sizeof(tbuf));        
            
            for(int i=0;i<bl;++i)
            {
                buf[c++] = tbuf[i];
            }
        }while(bl==1024);

        buf[c++] = '\r';
        
        write(cfd,buf,c);
        //n = read(cfd, buf, MAXLINE);        
        printf("Send %d bytes to server.\n",c-d-1);

        close(fd);
        
    }

    printf("exit\n");
    close(cfd);
    return 0;
}