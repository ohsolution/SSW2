
#include "../header/definition.h"
#include "../header/GlobalData.h"
#include "../header/FunctionHeader/search.h"
#include "../header/FunctionHeader/IO.h"
#include <stdio.h>

bool excute_query(void)
{    
    query input = ConsoleInput();          

    if(input.tp == WRONGQUERY) return true;

    osz = -1;

    search(input);    

    free(input.wstr);

    num_write(-1,-1);

    write(STDOUT,endl,sizeof(endl));

    return true;
}

int main(int argc,char **argv)
{    
    secure_open(argc,argv);
    
    save_offset();

    while(excute_query());

    return 0;
}
