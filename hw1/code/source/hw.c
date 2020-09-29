
#include "header/definition.h"
#include "header/GlobalData.h"
#include "header/FunctionHeader/search.h"
#include "header/FunctionHeader/IO.h"
#include "header/FunctionHeader/parser.h"

bool excute_query()
{
    query input = parsing(ConsoleInput());          

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
    
    secure_write(STDOUT,padding,sizeof(padding));

    return true;
}

int main(int argc,char **argv)
{
    int fd;
    
    secure_open(&fd,argc,argv);

    save_offset(fd);
    
    while(excute_query(fd));

    return 0;
}
