#include "../../header/GlobalData.h"
#include "../../header/definition.h"
#include <stdio.h>

void secure_open(int * fd,int argc,char **argv)
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
    
    *fd = open(argv[1],O_RDONLY);
        
    if(*fd<0)
    {
        write(STDOUT,err_c,25);
        exit(1);
    }

    return;
}

void secure_write(int fd,char * str,size_t sz)
{

}

void save_offset(int fd)
{
    offsetsz = (size_t *)malloc(sizeof(size_t)*(line));    
    tyline = (Qtype *)malloc(sizeof(Qtype)*(line));
    char buf[max_v];
    int bl;
    size_t spos =0;

    int space =0;
    int type=0;

    do
    {
        bl = read(fd,buf,sizeof(buf));

        for(int i=0;i<bl;++i,++spos)
        {
            if(buf[i] == '\n')
            {
                offsetsz[curline] = spos;
                spos =0;
                
                type += (!type) * (space+1); 
                tyline[curline] = type;
                type =0; space=0;

                if(++curline == line)
                {
                    line <<=1;
                    offsetsz = realloc(offsetsz,sizeof(size_t)*(line));
                    tyline = realloc(tyline,sizeof(Qtype)*(line));
                }
            }

            if(buf[i] == '*') type = REGULAREXP;
            if(buf[i] == '\"') type = SENENTENCE;
            space += (buf[i]==' ');

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

    ret.wordnum = 1;
    ret.word = 

    char buf[1];

    while(read(STDIN,buf,sizeof(buf))
    {
        if(buf[0] == ' ' || buf[0]=='\t') 
        {
            ++ret.wordnum;

        }
        else if(buf[0] == '\"') ret.type = SENTENCE;
        else if(buf[0] == '*') ret.type = REGULAREXP;
        else if(buf[0] =='\n');
        else
        {
            
        }
    }
        
}






















