#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/select.h>
#include <stdio.h>

#define MAX_SEATS 256
#define MAX_CLIENT 1024
#define MAX_LINE 256

/* static data define */
typedef enum _ACTION
{ 
    LOGIN = 1,
    RESERVE,
    CHECK_RESERVATION,
    CANCLE_RESERVATION,
    LOGOUT
    //TERMINATE    
}ACTION;

typedef struct _query 
{
    int user;
    int action;
    int data;
} query;

typedef struct _seat
{
    int user_id;
    pthread_mutex_t mutex;
} seat;

pthread_mutex_t overmutex;
seat arr[MAX_SEATS];
pthread_t tid[MAX_CLIENT];
int checker[MAX_CLIENT];
int upwd[MAX_CLIENT];

/* tool function define*/
int invalid(query q);

query parsing(char str[]);

int login(query q);
int rev(query q);
void check_rev(query q,char str[]);
int cancle_rev(query q);
int logout(query q);


/* thread movement */
void * client_manager(void *arg)
{
    int client_fd = *((int *)arg);
    int n;
    char buf[MAX_LINE];
    char ret;

    /* login */
    while((n = read(client_fd,buf,MAX_LINE)) > 0)
    {
        query q = parsing(buf);

        if(invaild(q)) 
        {
            // send invalid
            continue;
        }

        int ret = login(q);

        if(ret)
        {
            break;
        }
        else
        {

        }
    }

    /* handle opr */
    while((n = read(client_fd,buf,MAX_LINE)) > 0)
    {
        query q = parsing(buf);

        if(invaild(q)) 
        {
            //send invalid
            continue;
        }

        switch(q.action)
        {
            case RESERVE:
            break;
            case CHECK_RESERVATION:
            break;
            case CANCLE_RESERVATION:
            break;
            case LOGOUT:
            break;
            default:
            //send invalid
            break;
        }
        
    }

    pthread_mutex_lock(&overmutex);
    checker[client_fd] =0;
    pthread_mutex_unlock(&overmutex);
}

int main(int argc, char* argv[])
{
    /* init seat */
    for(int i=0;i<MAX_SEATS;++i)
    {
        arr[i].user_id = 0;
        pthread_mutex_init(&(arr[i].mutex),NULL);
    }

    pthread_mutex_init(&overmutex,NULL);

    int serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr,caddr;
    int cfd,caddrlen;
    int copyfd[MAX_CLIENT];

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[1]));
    serverAddr.sin_addr.s_addr = htons(INADDR_ANY);

    if(bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) return 0;

    if(listen(serverSocket, MAX_CLIENT) < 0) return 0;

    while(1)
    {
        caddrlen = sizeof(caddr);
        if((cfd = accept(serverSocket,(struct sockaddr *)&caddr,(socklen_t *)&caddrlen))<0) printf("accept() failed.\n");            
        else
        {            
            int d = -1;            
            pthread_mutex_lock(&overmutex);
            while(++d<MAX_LINE && checker[d]);
            checker[d] =1;
            pthread_mutex_unlock(&overmutex);

            copyfd[d] = cfd;
            pthread_create(&tid[d],NULL,client_manager,(void*)&copyfd[d]);
        }
    }

    for(int i=0;i<MAX_LINE;++i) pthread_join(tid[i],NULL);    

    return 0;
}

int invalid(query q)
{

}

query parsing(char str[])
{

}

int login(query q);
int rev(query q);
void check_rev(query q,char str[]);
int cancle_rev(query q);
int logout(query q);