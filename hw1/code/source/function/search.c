#include "../../header/definition.h"
#include "../../header/GlobalData.h"
#include "../../header/FunctionHeader/IO.h"

void opr_single(char * instr, char * oristr,size_t isz,size_t osz,int ln)
{
    int index = 0;
    while(1)
    {
        index = kmp(oristr+index,instr,isz,osz);

        if(index==-1) break;

        num_write(ln,index);

        index += (isz-1);
    }
    return;
}

void opr_multi(char * instr, char * oristr,size_t isz,size_t osz,int ln)
{
    int l = 0;
    bool ck = true;
    while(1)
    {
        int r = find_sp(oristr+l+1);

        if(r==-1) break;

        if(kmp(oristr,strcp(l,r),r-l+1,osz)==-1)
        {
            ck=false;
            break;
        };

        l = r;        
    }

    if(ck) num_write(ln,-1);

    return;
}

void opr_regex(char * instr, char * oristr,size_t isz,size_t osz,int ln)
{
    int l = 0;
    int r = find_sp(oristr+l+1);
    int z = find_sp(oristr+r+1);

    char * left = strcp(l,r);
    char * right = strcp(r,z);

    int lp = kmp(oristr,left,r-l+1,osz);
    int rp = kmp(oristr+lp+r-l+1,osz);

    bool ck = ((lp!=-1) && (rp!=-1));
  
    if(ck) num_write(ln,-1);

    return;
}

void search(query input)
{
    int off = 0;
    for(int i=1; i<curline; ++i,lseek(IFD,off,SEEK_SET))
    {
        char * buf = (char*)malloc(sizeof(char)*(offsetsz[i]+2));
        read(IFD,buf+1,offsetsz[i]);

        for(int t =1; t<offsetsz[i]+1;++t) if(buf[t] == '\t' || buf[t] == '\n') buf[t] = ' ';
        buf[offsetsz[i]+1] = '\n';
        
        switch(input.type)
        {
            case SINGLEWORD:
            case SENTENCE:                
                opr_single(input.wstr,buf,input.sz,offsetsz[i]+1,i);
                break;
            case MULTWORD:
                opr_multi(input.wstr,buf,input.sz,offsetsz[i]+1,i);
                break;            
            case REGULAREXP:
                opr_regex(input.wstr,buf,input.sz,offsetsz[i]+1,i);
                break;
            default:
                //error
        }


        off += offsetsz[i];
    }
}
