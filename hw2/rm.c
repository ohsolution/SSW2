#include "definition.h"
#include "errhandler.h"

int main(int argc,char * argv[])
{
    set_default();
    if(argc == 2) remove(argv[1]);
    err_check("rm");
    return 0;
}