#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define MAXLINE 64

int main (int argc, char *argv[]) 
{
    int n, listenfd, connfd, caddrlen;
    struct hostent *h;
    struct sockaddr_in saddr, caddr;
    char buf[MAXLINE];
    int port = atoi(argv[1]);

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        printf("socket() failed.\n");
        exit(1);
    }

    memset((char *)&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(port);

    if (bind(listenfd, (struct sockaddr *)&saddr,sizeof(saddr)) < 0) 
    {
        printf("bind() failed.\n");
        exit(2);
    }
    if (listen(listenfd, 5) < 0) 
    {
        printf("listen() failed.\n");
        exit(3);
    }


    while (1) 
    {
        caddrlen = sizeof(caddr);
        if ((connfd = accept(listenfd, (struct sockaddr *)&caddr,(socklen_t *)&caddrlen)) < 0) 
        {
            printf ("accept() failed.\n");
            continue;
        }
        
        while ((n = read(connfd, buf, MAXLINE)) > 0) 
        {
            if(buf[0]=='q' && buf[1]=='u' && buf[2]=='i' && buf[3]=='t' && buf[4]=='\n')
            {
                close(connfd);
                return 0;
            }

            //printf ("got %d bytes from client.\n", n);

            char filename[77];
            char filebuf[200057];
            int c=-1;
            int d=0;

            while(buf[++c]!='>') filename[c] = buf[c];
            filename[c] = '_';
            filename[c+1] = 'c';
            filename[c+2] = 'o';
            filename[c+3] = 'p';
            filename[c+4] = 'y';
            filename[c+5] = '\0';

            ++c;
            do
            {                
                int ck =0;
                for(int i=c;i<n;++i) 
                {
                    if(buf[i]=='\r')
                    {
                        ck =1;
                        break;
                    }
                    filebuf[d++] = buf[i];                    
                }
                c=0;
                if(ck) break;

            } while ((n = read(connfd, buf, MAXLINE)) > 0);            

            printf ("got %d bytes from client.\n", d);
            //printf("%s\n",filebuf);

            int ofd = open(filename, O_RDWR | O_CREAT , 0644);

            write(ofd,filebuf,d);

            /*
            char tbuf[1024];
            int bl=0;
            int o= 0;

            while(d > o)
            {
                int u=-1;
                while(filebuf[++u+o]&&u!=4) tbuf[u] = filebuf[u+o];
                write(ofd,tbuf,u);
                o += 1024;
            }
            */
            close(ofd);
            //write(STDOUT_FILENO,buf,n);
            //write(connfd, buf, n);            
        }
        close(connfd);
    }

    return 0;
}