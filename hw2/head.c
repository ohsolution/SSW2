#include "definition.h"
#include "errhandler.h"

int main(int argc,char * argv[])
{
    set_default();

    int otk = 0;
    FILE * inf = NULL;

    if(argc > 2 && !strcmp(argv[1],"-n")) otk = atoi(argv[2]);

    if((otk && argc == 4) || (!otk && argc==2)) 
    {
        inf = fopen(argv[3-(!!!otk)*2],"r");
        if(!inf)
        {
            err_check("head");
            return 0;
        }
    }

    if(!otk) otk = 10;
    
    char * line = NULL;
    size_t line_sz = 0;
    ssize_t read;
    
    while((read = getline(&line,&line_sz,(inf?inf:stdin)))!=-1 && otk--) printf("%s",line);        
    
    err_check("head");
    
    return 0;
}