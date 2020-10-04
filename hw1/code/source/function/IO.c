#include "../../header/GlobalData.h"
#include "../../header/definition.h"

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
        int t = ln % 10;
        output[i] = (char)(t+'0');
    }
    output[osz+sz] = ' ';
    osz = osz+sz+1;
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

    output[osz-1] = ':';

    if(index) to_str(index,getsz(index));    
    else 
    {
        output[osz] = '0';
        output[++osz] = ' ';
        ++osz;
    }

}


void save_offset()
{
    offsetsz = (size_t *)malloc(sizeof(size_t)*(line));    
    char buf[max_v];
    int bl;
    size_t spos = 0;

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
    ++offsetsz[1];
   return;
}

query ConsoleInput(void)
{
    query ret;

    ret.wstr = (char*)malloc(sizeof(char)*max_v);
    
    int bl;
    size_t spos = 0;
    
    bool ck = false;
    bool sck = true;
    bool wck = false;

    size_t sl=max_v;
    int wsz = 0;
    ret.wstr[spos++] = ' ';
    ret.tp = 0;

    int star = 0;
    int ldash = -1;
    int rdash = -1;
    int dash = 0;
    int palse = 0;
    bool wsign = false;

    do
    {
        bl = read(STDIN,ret.wstr+spos,sizeof(char)*max_v);

        for(int i=0;i<bl;++i,++spos)
        {
            if(ret.wstr[spos]!=' ' && ret.wstr[spos]!='\t'&&ret.wstr[spos]!='\n' && ret.wstr[spos]!='*' && ret.wstr[spos]!='\"')
            {
                wck=true;
                if(palse>=2 && wsz >=1) wsign = true;
                palse = 0;
            }
            else 
            {
                if(wck) ++wsz;
                ++palse;
                wck=false;
            }

            star += (ret.wstr[spos] == '*');

            ck |= (ret.wstr[spos] == '\n');

            if(ret.wstr[spos] == '*') ret.tp = REGULAREXP;
            if(ret.wstr[spos] == '\"') ret.tp = SENTENCE;

            if(ret.wstr[spos]=='*' || ret.wstr[spos] == '\n') ret.wstr[spos]=' ';

            if(ret.wstr[spos]=='\"') 
            {
                sck=false;
                if(ldash == -1) ldash = spos;
                else rdash = spos;
            }

            if(ret.wstr[spos]=='\t' && sck) ret.wstr[spos] =' ';

            if(spos+1==sl) 
            {                
                sl <<=1;
                void * tmp = realloc(ret.wstr,sl);
                if(!tmp)
                {
                    /*
                    write(STDOUT,err_d,25);
                    exit(1);*/
                }
                else ret.wstr = tmp;     
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

    }while(!ck);
    
    ret.sz = spos+1;

    void * tmp = realloc(ret.wstr,sizeof(char) * ret.sz);
    if(!tmp); // error
    else ret.wstr =tmp;

    ret.wstr[ret.sz-1] = '\n';
    ret.wordsz = wsz;
    if(!ret.tp) ret.tp = (ret.wordsz>1)+1;
    if(ret.tp == REGULAREXP && (star!=1||wsz!=2)) ret.tp = WRONGQUERY;    
    if(ret.tp==MULTWORD && wsign) ret.tp = WRONGQUERY;
    if(dash && star) ret.tp = WRONGQUERY;

    if(ret.tp==SENTENCE)
    {
        if(ldash==-1 || rdash==-1)
        {
            ret.tp = WRONGQUERY;
        }
        else
        {
            ret.wstr[ldash] = '*';
            ret.wstr[rdash] = '*';
            bool senck = true;
            for(int i = ldash-1;i>=0;--i)
            {
                if(ret.wstr[i]!=' ' && ret.wstr[i]!='\t'&&ret.wstr[i]!='\n' && ret.wstr[i]!='*' && ret.wstr[i]!='\"')
                {
                    senck=false;
                    break;
                }   
            }
            for(int i = rdash+1;i<ret.sz;++i)
            {
                if(ret.wstr[i]!=' ' && ret.wstr[i]!='\t'&&ret.wstr[i]!='\n' && ret.wstr[i]!='*' && ret.wstr[i]!='\"')
                {
                    senck=false;
                    break;
                }   
            }

            if(!senck) ret.tp= WRONGQUERY;            
        }    
    }

    if(ret.tp == SENTENCE)
    {
        if(ret.wstr[ldash+1]==' ' || ret.wstr[ldash+1]=='\t') ret.wstr[ldash] = 17;
        if(ret.wstr[rdash-1]==' ' || ret.wstr[rdash-1]=='\t') ret.wstr[rdash] = 17;
    }
    return ret;
}
