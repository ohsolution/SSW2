#include "header/definition.h"
//#include "header/FunctionHeader/IO.h"
//#include "header/GlobalData.h"
#include <stdio.h>

int main(int argc,char **argv)
{
    char * att;
    int fn = 16;

    att = (char *)malloc(sizeof(char)*fn);

    read(STDIN,att,sizeof(att));

    att= realloc(att,sizeof(char)*8);

    printf("%s",att);

    return 0;
}
