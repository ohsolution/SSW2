#include "../../header/GlobalData.h"
#include "../../header/definition.h"
#include <stdio.h>

void secure_open(int argc,char **argv)
{
    if(argc < 2)
    {
        write(STDOUT,err_a,20);
        exit(1);
    }
    else if(argc > 2)
    {
        write(STDOUT,err_b,20);
        exit(1);
    }
    
    IFD = open(argv[1],O_RDONLY);
        
    if(IFD<0)
    {
        write(STDOUT,err_c,25);
        exit(1);
    }

    return;
}

void secure_write(int fd,char * str,size_t sz)
{

}

void save_offset()
{
    offsetsz = (size_t *)malloc(sizeof(size_t)*(line));    
    char buf[max_v];
    int bl;
    size_t spos =0;

    do
    {
        bl = read(IFD,buf,sizeof(buf));

        for(int i=0;i<bl;++i,++spos)
        {
            if(buf[i] == '\n')
            {
                offsetsz[curline++] = spos;
                spos =0;

                if(curline == line)
                {
                    line <<=1;
                    offsetsz = realloc(offsetsz,sizeof(size_t)*(line));
                }
            }
        }

        /*
        #error
        if(bl<0)
        {
            write(STDOUT,err_d,25);
            exit(1);
        }
        */

    }while(bl==max_v);

   return;
}

query ConsoleInput(void)
{
    query ret;

    ret.wstr = (char*)malloc(sizeof(char)*max_v);
    ret.wstr[0] = ' ';
    int bl;
    size_t spos =1;
    ret.type = 0;
    int space =0;

    do
    {
        bl = read(STDIN,ret.wstr+spos,sizeof(char)*max_v);

        for(int i=0;i<bl;++i,++spos)
        {
            if(ret.wstr[spos] == '\n') ret.type += (!ret.type) * (space+1);
            if(ret.wstr[spos] == '*') ret.type = REGULAREXP;
            if(ret.wstr[spos] == '\"') ret.type = SENTENCE;
            space += (ret.wstr[spos]==' ' || ret.wstr[spos] == '\t');
            
            if(ret.wstr[spos] == '\n' || ret.wstr[spos]=='\t' || ret.wstr[spos]=='*' || ret.wstr[spos]=='\"') ret.wstr[spos]=' ';
            if(spos==sizeof(ret.wstr)) ret.wstr = realloc(ret.wstr,sizeof(ret.wstr)*2);            
        }

        /*
        #error
        if(bl<0)
        {
            write(STDOUT,err_d,25);
            exit(1);
        }
        */

    }while(bl==max_v);

    ret.sz = spos;

    return ret;
}
