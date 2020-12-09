#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>

#define MAX_LINE 1024
#define MAX_SEATS 256

typedef enum _ACTION
{ 
    LOGIN = 1,
    RESERVE,
    CHECK_RESERVATION,
    CANCLE_RESERVATION,
    LOGOUT,
    INVALID,
    TERMINATE    
}ACTION;

typedef enum _bool
{
    false,
    true
}bool;

typedef struct _result 
{
    int action;
    int val;
    bool data[MAX_SEATS];
    int idx;    
} result;

/* tool function define */
result * parsing(char str[],int n);
bool print_result(result *r);

int main (int argc, char *argv[])
{
    int client_socket;
    struct sockaddr_in server_addr;
    struct hostent *h = gethostbyname(argv[1]);
    FILE * ifd;
    char * buf;

    if (argc == 3) ifd = stdin;
    else if (argc == 4) ifd = fopen(argv[3],"r");     
    else 
    {
        printf("Follow the input rule below\n");
        printf("==================================================================\n");
        printf("argv[1]: server address, argv[2]: port number\n");
        printf("or\n");
        printf("argv[1]: server address, argv[2]: port number, argv[3]: input file\n");
        printf("==================================================================\n");
        exit(1);
    }

    if((client_socket = socket(PF_INET, SOCK_STREAM, 0))<0)
    {
        printf("socket() failed.\n");
        exit(2);
    }

    memset((char *)&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    memcpy((char*)&server_addr.sin_addr.s_addr,(char *)h->h_addr,h->h_length);
    server_addr.sin_port = htons(atoi(argv[2]));

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Connection failed\n");
        exit(3);
    }

    size_t tli= MAX_LINE;
    ssize_t n;
    bool term = false;

    while((n=getline(&(buf),&tli,ifd))!=-1)
    {
        write(client_socket,buf,n);
        n = read(client_socket,buf,MAX_LINE);
        
        result * ret = parsing(buf,n);

        if(print_result(ret)) 
        {
            term = true;
            break;
        }   
    }

    if(ifd != stdin && !term)    
    {
        write(client_socket,"0 0 0",6);
        n = read(client_socket,buf,MAX_LINE);
    }
	    
    close(client_socket);

    return 0;
}

result * parsing(char str[],int n)
{
    char tmp[30];
    result * ret = (result *)malloc(sizeof(result));
    bool act = false;
    int c = 0;
    int idx = 0;

    for(int i=0;i<n;++i)
    {
        if(str[i] == ' ' || str[i] == '\0')
        {
            tmp[c] = '\0';

            if(!act)
            {                
                ret->action = atoi(tmp);
                act = true;
            }
            else
            {
                if(ret->action == CHECK_RESERVATION) ret->data[idx++] = atoi(tmp);
                else ret->val = atoi(tmp);                
            }

            c=0;            
        } 
        else tmp[c++] = str[i];
    }

    return ret;
}

bool print_result(result * r)
{
    bool ret = false;

    switch (r->action)
    {
    case LOGIN:
        if(r->val == 1) printf("Login success\n");
        else if(r->val == -1) printf("Login failed\n");
        break;
    case RESERVE:
        if(r->val == -1) printf("Reservation failed\n");
        else printf("Seat %d is reserved\n",r->val);
        break;
    case CHECK_RESERVATION:
        if(r->data[0] == -1) printf("Reservation check failed\n");
        else
        {
            printf("Reservation: ");

            int last = MAX_SEATS;
            while(!r->data[--last]);
            for(int i=0;i<MAX_SEATS;++i) if(r->data[i] && i!=last) printf("%d, ",i);
            printf("%d\n",last);
        }
        break;
    case CANCLE_RESERVATION:
        if(r->val == -1) printf("Cancel failed\n");
        else printf("Seat %d is canceled\n",r->val);
        break;
    case LOGOUT:
        if(r->val == 1) printf("Logout success\n");
        else if(r->val == -1) printf("Logout failed\n");   
        break;
    case INVALID:
        printf("Invalied query\n");
        break;
    case TERMINATE:
        ret = (r->val == 256);
        break;    
    default:
        break;
    }

    free(r);
    return ret;
}
