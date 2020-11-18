#include "definition.h"
#include "errhandler.h"

int main(int argc,char * argv[])
{
    set_default();
    if(argc==1) fprintf(stderr,"mv: missing file operand\n");
    else if(argc==2) fprintf(stderr,"mv: missing destination file operand after %s\n",argv[1]);   
    else
    {
        rename(argv[1],argv[2]);
    }
    
    err_check("mv");
    return 0;
}