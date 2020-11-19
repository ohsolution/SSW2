#include "definition.h"
#include "errhandler.h"

int main(int argc,char * argv[])
{
    set_default();
    
    FILE * inf = NULL;

    if(argc == 2) inf = fopen(argv[1],"r");
    
    char * line = NULL;
    size_t line_sz = 0;
    ssize_t read;

    if(inf)
    {
        while((read = getline(&line,&line_sz,(inf?inf:stdin)))!=-1) printf("%s",line);            
        fclose(inf);
    }
    err_check("cat");    
    
    return 0;
}