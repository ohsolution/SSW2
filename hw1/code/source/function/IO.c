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

int getsz(int ln)
{
    int ret = 0;
    while(ln)
    {
        ln/=10;
        ++ret;
    }
    return ret;
}

void to_str(int ln,int sz)
{
    for(int i=osz+sz-1;i>=osz;--i,ln/=10)
    {
        output[i] = (ln%10)-'0';
    }
    output[osz+sz] = ' ';
    osz = osz+sz;
    return;
}

void num_write(int ln,int index)
{
    if(ln == -1 && osz!=-1)
    {
        write(STDOUT,output,--osz);
        return;
    }

    if(osz!=-1) write(STDOUT,output,osz);

    osz= 0;

    to_str(ln,getsz(ln));    

    if(index==-1) return;

    output[osz++] = ':';

    to_str(index,getsz(index));
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
                    void * tmp =realloc(offsetsz,sizeof(size_t)*(line));
                    if(!tmp)
                    {
                        write(STDOUT,err_d,25);
                        exit(1);
                    }
                    offsetsz = tmp;
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
    
    int bl;
    size_t spos = 0;
    ret.type = 0;
    int space =0;
    
    bool ck = false;
    bool sck = true;
    bool wck = false;

    size_t sl=max_v;

    ret.wstr[spos++] = ' ';
    
    do
    {
        bl = read(STDIN,ret.wstr+spos,sizeof(char)*max_v);

        for(int i=0;i<bl;++i,++spos)
        {
            ck |= (ret.wstr[spos] == '\n');

            if(ret.wstr[spos] == '\n') ret.type += (!ret.type) * ((space>0) + 1);
            if(ret.wstr[spos] == '*') ret.type = REGULAREXP;
            if(ret.wstr[spos] == '\"') ret.type = SENTENCE;
            if(wck) space += (ret.wstr[spos]==' ' || ret.wstr[spos] == '\t');
            
            if(ret.wstr[spos]=='*' || ret.wstr[spos] == '\n') ret.wstr[spos]=' ';

            if(ret.wstr[spos]=='\"') 
            {
                ret.wstr[spos] = '*';
                sck = !sck;
            }

            if(ret.wstr[spos]=='\t' && sck) ret.wstr[spos] = ' '; 

            if(spos+1==sl) 
            {                
                sl <<=1;
                void * tmp = realloc(ret.wstr,sl);
                if(!tmp)
                {
                    write(STDOUT,err_d,25);
                    exit(1);
                }
                else ret.wstr = tmp;     
            }

            if(ret.wstr[spos]!=' ' && ret.wstr[spos]!='\t'&&ret.wstr[spos]!='\n' && ret.wstr[spos]!='*' && ret.wstr[spos]!='\"') wck=true;            
        }

        /*
        #error
        if(bl<0)
        {
            write(STDOUT,err_d,25);
            exit(1);
        }
        */

    }while(!ck);

    ret.sz = spos+1;

    void * tmp = realloc(ret.wstr,sizeof(char) * ret.sz);
    if(!tmp); // error
    else ret.wstr =tmp;


    ret.wstr[ret.sz-1] = '\n';

    return ret;
}
