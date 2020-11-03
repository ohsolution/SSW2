#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    printf("start\n");

    if(fork()==0)
    {
        for(int i=0;i<51;++i) printf("2\n");
        exit(0);
    }
    else
    {
        for(int i=0;i<7;++i) printf("1\n");
        //wait(NULL);
        for(int i=0;i<15;++i) printf("3\n");
    }
    
    printf("out\n");
    return 0;
}
