#include "../../header/definition.h"
#include "../../header/GlobalData.h"
#include "../../header/FunctionHeader/IO.h"

bool match(char a,char b)
{
    if(a==b) return true;

    if(a >= 'A' && a <='Z' && b-a==32) return true;

    if(a >= 'a' && a <='z' && ((int)a-(int)b)==32) return true;

    if(a=='*') return (b==' ' || b=='\t');

    return false;
}


int find_ep(char * str,int idx,char ch)
{
    bool ck = false;

    while(*(str+idx)!='\n')
    {
        if(*(str+idx)==ch) ck = true;
        else if(ck) return idx-1;
        ++idx;
    }

    return -1;
}

int find_sp(char * str,int idx,char ch)
{
    while(*(str+idx)!='\n')
    {
        if(*(str+idx)==ch) return idx;
        ++idx;
    }

    return -1;
}

int find_str(char * ori, char * w,int lsz,int lp)
{
    int t= 0;

    for(char * it = (ori+lsz) ; *it!='\n';++it)
    {
        bool ck =true;
        for(char *lt = w,t=0; t<lp;++t,++lt)
        {
            if(match(*lt,*(it+t))) continue;
            ck = false;
            break;
        }
        if(ck) return it-ori;
    }

    return -1;
}

char * strcp(char * str, int l ,int r)
{
    char * tmp = malloc(r-l+1);

    int c=  l;
    int d = 0;

    while(c<=r)
    {
        tmp[d++] = *(str+c);
        ++c;
    }

    return tmp;
}

void opr_single(char * instr, char * oristr,int ln)
{
    int index = 0;

    int l = find_ep(instr,0,' ');
    int r = find_sp(instr,l+1,' ');
    int wsz = r-l;
    char * incp = strcp(instr,l,r);

    while(1)
    {
        index = find_str(oristr,incp,index,r-l+1);

        if(index==-1) break;

        num_write(ln,index);

        index += wsz;
    }
    
    free(incp);
    return;
}

void opr_multi(char * instr, char * oristr,int ln,int wsz)
{
    bool ck = true;
    int l,r;
    l = r = 0;
    int c =0;
    while(1)
    {   
        l = find_ep(instr,r,' ');
        r = find_sp(instr,l+1,' ');
        char * incp = strcp(instr,l,r);

        if(find_str(oristr,incp,0,r-l+1) == -1)
        {
            ck = false;
            free(incp);
            break;
        }

        free(incp);        

        if(++c == wsz) break;
    }

    if(ck) num_write(ln,-1);
    

    return;
}

void opr_regex(char * instr, char * oristr,int ln)
{
    int l,r;
    l = r = 0;

    l = find_ep(instr,r,' ');
    r = find_sp(instr,l+1,' ');
    int wsz = r-l;
    char * lcp = strcp(instr,l,r);
    
    l = find_ep(instr,r,' ');
    r = find_sp(instr,l+1,' ');
    char * rcp = strcp(instr,l,r);
    
    int lidx = find_str(oristr,lcp,0,wsz+1);
    int ridx = find_str(oristr,rcp,lidx+wsz+1,r-l+1);

    bool ck = ((lidx!=-1) && (ridx!=-1));
  
    if(ck) num_write(ln,-1);

    free(lcp);
    free(rcp);

    return;
}

void opr_sentence(query input, char * oristr,int ln)
{
    int l,r;
    
    l = input.ldash;
    r = input.rdash;
    bool lpad = false;;
    if(l<0) {l*=-1;++l;lpad=true;}
    if(r<0) {r*=-1;--r;}

    char * incp = strcp(input.wstr,l,r);

    int index= -1;

    while(1)
    {
        index = find_str(oristr,incp,index+1,r-l+1);

        if(index==-1) break;
        
        num_write(ln,index-lpad);
    }

    free(incp);

    return;
}

void search(query input)
{
    int off = 0;
    lseek(IFD,0,SEEK_SET);

    for(int i=1; i<curline; ++i)
    {
        char * buf = (char*)malloc(sizeof(char)*(offsetsz[i]+2));
        
        read(IFD,buf+1,offsetsz[i]+1);
        buf[0] = ' ';
        buf[offsetsz[i]] = ' '; 
        buf[offsetsz[i]+1] = '\n';        

        if(input.tp != SENTENCE)
        {
            for(int t =0; t<=offsetsz[i]+1;++t) 
            {
                if(buf[t] == '\t') buf[t] = ' ';
                
            }
        }

        switch(input.tp)
        {
            case SINGLEWORD:
                opr_single(input.wstr,buf,i);
                break;
            case MULTWORD:
                opr_multi(input.wstr,buf,i,input.wordsz);
                break;            
            case REGULAREXP:
                opr_regex(input.wstr,buf,i);
                break;
            case SENTENCE:                
                opr_sentence(input,buf,i);
                break;
            default:
                break;
                //error
        }
        
        off += offsetsz[i];
        lseek(IFD,off,SEEK_SET); 
        free(buf);
    }
}
