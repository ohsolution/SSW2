
#include "../header/definition.h"
#include "../header/GlobalData.h"
#include "../header/FunctionHeader/search.h"

//#include "../header/FunctionHeader/parser.h"
#include "../header/FunctionHeader/IO.h"

#include <stdio.h>

bool excute_query(void)
{    
    query input = ConsoleInput();          

    osz = -1;

    //write(STDOUT,input.wstr,input.sz);
    
    //printf("%d %d %d\n",input.wordsz,input.tp,input.sz);
    //write(STDOUT,input.wstr,input.sz);

    search(input);
    
    //printf("clear");

    /*
    switch(input.type)
    {
        case SINGLEWORD:
            break;
            search_singleword(input);
        case MULTWORD:
            search_multword(input);
            break;
        case SENTENCE:
            search_sentence(input);
            break;
        case REGULAREXP:
            search_regularexp(input);
            break;
        case WRONGQUERY:
            // output error?
            break;
    }
    */

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
