#include "definition.h"
#include "errhandler.h"

int main(int argc,char * argv[])
{
    set_default();
    
    FILE * inf = NULL;

    if(argc == 2) inf = fopen(argv[1],"r");

    if(argc==2 && !inf)
    {
        err_check("cat");
        return 0;
    }
    
    char * line = NULL;
    size_t line_sz = 0;
    ssize_t read;

    while((read = getline(&line,&line_sz,(inf?inf:stdin)))!=-1) printf("%s",line);            
    if(inf) fclose(inf);

    err_check("cat");    
    
    return 0;
}