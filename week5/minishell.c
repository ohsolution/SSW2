#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

const int max_v= 1e4+7;

int query()
{
    int c = 0;
    char path[max_v];
    char * cmd;

    size_t size = 0;
    if(getline(&cmd,&size,stdin)==1) return 1;
    char * ptr = cmd;
    int sz = strlen(cmd)-1;
    cmd[sz] = '\0';
    
    char * pch = strtok(cmd," ");
    char ** args = (char**)malloc(sizeof(char*)*sz);    


    while(pch)
    {
        args[c++] = pch;
        pch = strtok(NULL," ");
    }
    args[c] = NULL;

    
    sprintf(path,"/bin/%s",args[0]);
    
    if(!strcmp(args[0],"quit")) goto EXIT;
    else 
    {
        if(fork() == 0)
        {
            execv(path,args);
            exit(0);
        }
        else wait(NULL);
    }
    
    free(args);
    free(ptr);
    return 1;

    EXIT:
    free(args);
    free(ptr);
    return 0;
}

int main()
{
    while(query());
    return 0;
}
