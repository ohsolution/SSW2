#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>

#define MAX_LINE 256

typedef enum _ACTION
{ 
    LOGIN = 1,
    RESERVE,
    CHECK_RESERVATION,
    CANCLE_RESERVATION,
    LOGOUT    
}ACTION;

typedef struct _result 
{
    int action;
    int data[MAX_LINE];    
} result;


/* tool function define */
result * parsing(char str[]);
void print_result(result *r);

int main (int argc, char *argv[])
{
    int client_socket,ifd,n;
    struct sockaddr_in server_addr;
    struct hostent *h = gethostbyname(argv[1]);
    char buf[MAX_LINE];

    if (argc == 3) ifd = STDIN_FILENO;
    else if (argc == 4) ifd = open(argv[3],O_RDONLY);
     
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

    while((n=read(ifd,buf,MAX_LINE)) >0)
    {
        write(client_socket,buf,n);

        n = read(client_socket,buf,MAX_LINE);

        result * ret = parsing(buf);

        print_result(ret);
    };

    if(ifd != STDOUT_FILENO)
    {
        // send TERMINATE to server
    }
	    
    close(client_socket);

    return 0;
}

result * parsing(char str[])
{

}

void print_result(result * r)
{
        /*
     * Insert your PA3 client code
     *
     * You should handle input query
     *
     * Follow the print format below
     *
     * 1. Log in
     * - On success
     *   printf("Login success\n");
     * - On fail
     *   printf("Login failed\n");
     *
     * 2. Reserve
     * - On success
     *   printf("Seat %d is reserved\n");
     * - On fail
     *   printf("Reservation failed\n");
     *
     * 3. Check reservation
     * - On success
     *   printf("Reservation: %s\n");
     *   or
     *   printf("Reservation: ");
     *   printf("%d, ");
     *   ...
     *   printf("%d\n");
     * - On fail
     *   printf("Reservation check failed\n");
     *
     * 4. Cancel reservation
     * - On success
     *   printf("Seat %d is canceled\n");
     * - On fail
     *   printf("Cancel failed\n");
     *
     * 5. Log out
     * - On success
     *   printf("Logout success\n");
     * - On fail
     *   printf("Logout failed\n");
     *
     */

}
