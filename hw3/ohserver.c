#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/select.h>
#include <stdio.h>
#include <string.h>

#define MAX_SEATS 256
#define MAX_CLIENT 1024
#define MAX_LINE 1024

/* static data define */
typedef enum _ACTION
{ 
    LOGIN = 1,
    RESERVE,
    CHECK_RESERVATION,
    CANCLE_RESERVATION,
    LOGOUT
}ACTION;

typedef enum _bool
{
    false,
    true
}bool;

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

pthread_t tid[MAX_CLIENT];

/* for client manage */
pthread_mutex_t overmutex; 
bool checker[MAX_CLIENT];

/* for login manage */
pthread_mutex_t loginmutex; 
int user_pw[MAX_CLIENT];
int user_ck[MAX_CLIENT];

/* for seat manage */
seat arr[MAX_SEATS];

/* tool function define*/
bool invalid(query q){return ((q.user < 0 || q.user > 1023) || (q.action < 1 || q.action > 5) || q.data==-1);}
bool terminate(query q){return !(q.action || q.data || q.user);}
query parsing(char str[],int n);

/* query action handle function */
bool login(query q,int id,int cfd);
void rev(query q,int cfd);
void check_rev(query q,int cfd);
void cancle_rev(query q,int cfd);
bool logout(query q,int id,int cfd);

/* thread movement */
void * client_manager(void *arg)
{
    int client_fd = *((int *)arg);
    ssize_t n;
    char buf[MAX_LINE];
    int id = -1;

    /* handle opr */
    while((n = read(client_fd,buf,MAX_LINE)) > 0)
    {
        //write(STDOUT_FILENO,buf,n); // DEBUG

        bool inv = false;
        int spcheck = 0;
        for(int i=0;i<n;++i)
        {
            spcheck += buf[i]==' ';
            if((buf[i]>='0' && buf[i] <='9') || (buf[i]=='\n' || buf[i]=='\0' || buf[i] == ' ')) continue;
            inv = true;
        }

        if(spcheck != 2) inv = true;

        if(inv)
        {
            write(client_fd,"6",2);
            continue;
        } 

        query q = parsing(buf,n);

        // printf("user : %d , action : %d, data : %d\n",q.user,q.action,q.data); // DEBUG

        if(terminate(q))
        {            
            if(id != -1) 
            {
                q.user = id;
                logout(q,id,client_fd);
            }

            write(client_fd,"7 256",6);
            break;
        }
        else if(invalid(q)) 
        {
            write(client_fd,"6",2);
            continue;
        }

        if((id!=-1 && id!=q.user) || (id == -1 && q.action != LOGIN))
        {
            char tmp[30];
            int t = sprintf(tmp,"%d -1",q.action)+1;
            write(client_fd,tmp,t);
            continue;
        }

        switch(q.action)
        {
            case LOGIN:
                if(login(q,id,client_fd)) id = q.user;
                break;
            case RESERVE:
                rev(q,client_fd);
                break;
            case CHECK_RESERVATION:
                check_rev(q,client_fd);
            break;
            case CANCLE_RESERVATION:
                cancle_rev(q,client_fd);
            break;
            case LOGOUT:
                if(logout(q,id,client_fd)) id = -1;
            break;
            default:            
                write(client_fd,"6",2);
            break;
        }
        
    }

    pthread_mutex_lock(&overmutex);
    checker[client_fd] =false;
    pthread_mutex_unlock(&overmutex);

    return NULL;
}

int main(int argc, char* argv[])
{
    /* init seat */
    for(int i=0;i<MAX_SEATS;++i)
    {
        arr[i].user_id = -1;
        pthread_mutex_init(&(arr[i].mutex),NULL);
    }

    pthread_mutex_init(&overmutex,NULL);
    pthread_mutex_init(&loginmutex,NULL);

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
            checker[d] =true;
            pthread_mutex_unlock(&overmutex);

            copyfd[d] = cfd;
            pthread_create(&tid[d],NULL,client_manager,(void*)&copyfd[d]);
        }
    }

    for(int i=0;i<MAX_LINE;++i) pthread_join(tid[i],NULL);

    for(int i = 0;i< MAX_SEATS;++i) pthread_mutex_destroy(&arr[i].mutex);

    pthread_mutex_destroy(&overmutex);
    pthread_mutex_destroy(&loginmutex);        
    close(serverSocket);

    return 0;
}

query parsing(char str[],int n)
{
    char tmp[30];
    int c = 0;
    int t = 1;
    query ret = {-1,-1,-1};

    for(int i=0;i<n;++i)
    {
        if(str[i] == ' ' || str[i]=='\n')
        {
            tmp[c] = '\0';
            
            switch (t)
            {
            case 1:
                ret.user = atoi(tmp);
                break;
            case 2:
                ret.action = atoi(tmp);
                break;
            case 3:
                ret.data = atoi(tmp);
                break;            
            default:
                break;
            }
            ++t;
            c=0;
            
        } 
        else tmp[c++] = str[i];
    }

    return ret;
}

bool login(query q,int id,int cfd)
{
    bool success = false;

    pthread_mutex_lock(&loginmutex);

    int status = user_ck[q.user];

    switch (status)
    {
    case -1: // can login
        success = (q.data == user_pw[q.user]);
        if(success) user_ck[q.user] = 1;
        break;
    case 0: // sign up
        user_pw[q.user] = q.data;
        user_ck[q.user] = 1;
        success = true;
        break;
    case 1: // can't login        
    default:
        break;
    }

    pthread_mutex_unlock(&loginmutex);

    if(success) write(cfd,"1 1",4);
    else write(cfd,"1 -1",5);

    return success;
}

bool logout(query q,int id,int cfd)
{
    pthread_mutex_lock(&loginmutex);
    user_ck[q.user] = -1;
    pthread_mutex_unlock(&loginmutex);

    if(q.action == LOGOUT) write(cfd,"5 1",4);

    return true;    
}

void rev(query q,int cfd)
{
    if(q.data < 0 || q.data > 255) // overflow
    {
        write(cfd,"2 -1",5);
        return;
    }

    bool ret = false;

    pthread_mutex_lock(&arr[q.data].mutex);
    ret = (arr[q.data].user_id==-1);
    if(ret) arr[q.data].user_id = q.user;
    pthread_mutex_unlock(&arr[q.data].mutex);

    char tmp[30];
    int t = sprintf(tmp,"2 %d",q.data)+1;    
    if(ret) write(cfd,tmp,t);
    else write(cfd,"2 -1",5);
}

void cancle_rev(query q,int cfd)
{
    if(q.data < 0 || q.data > 255) // overflow
    {
        write(cfd,"4 -1",5);
        return;
    }

    bool ret = false;

    pthread_mutex_lock(&arr[q.data].mutex);
    ret = (arr[q.data].user_id == q.user);
    if(ret) arr[q.data].user_id = -1;
    pthread_mutex_unlock(&arr[q.data].mutex);

    char tmp[30];
    int t = sprintf(tmp,"4 %d",q.data)+1;
    if(ret) write(cfd,tmp,t);
    else write(cfd,"4 -1",5);

    return;
}

void check_rev(query q,int cfd)
{
    bool ckr[MAX_SEATS];
    bool fck = false;

    for(int i=0; i<MAX_SEATS;++i)
    {
        pthread_mutex_lock(&arr[i].mutex);
        ckr[i] = (arr[i].user_id == q.user);
        fck |= ckr[i];
        pthread_mutex_unlock(&arr[i].mutex);
    }

    if(!fck) write(cfd,"3 -1",5);
    else
    {
        char str[MAX_LINE] = "3 ";

        for(int i=0;i<MAX_SEATS;++i)
        {            
            if(ckr[i]) strcat(str,"1 ");
            else strcat(str,"0 ");
        }
        
        write(cfd,str,strlen(str)+1);    
        
    }
}