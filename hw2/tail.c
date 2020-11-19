#include "definition.h"
#include "errhandler.h"
#include "cvector.h"


int main(int argc,char * argv[])
{
    set_default();

    int otk = 0;
    FILE * inf = NULL;

    if(argc > 2 && !strcmp(argv[1],"-n")) otk = atoi(argv[2]);

    if((otk && argc == 4) || (!otk && argc==2)) 
    {
        inf = fopen(argv[3-(!!!otk)*2],"r");
        if(!inf)
        {
            err_check("tail");
            return 0;
        }
    }

    if(!otk) otk = 10;

    char * line = NULL;
    size_t line_sz = 0;
    ssize_t read;
    int p = 0;

    if(inf)
    {
        int usz=0;
        cvector vsz;
        cv_init(&vsz,otk+1);
        vsz.arr[0] = 0;
        ++p;
        
        while((read = getline(&line,&line_sz,inf))!=-1)
        {
            if(!read) break;
            usz += read;
            vsz.arr[p] = usz;
            if(++p==otk+1) p = 0;
        }

        for(int i=p;i<otk+1;++i)
        {
            fseek(inf,vsz.arr[i],0);
            getline(&line,&line_sz,inf);
            printf("%s",line);
        }

        for(int i=0;i<p-1;++i)
        {
            fseek(inf,vsz.arr[i],0);
            getline(&line,&line_sz,inf);
            printf("%s",line);
        }

        if(inf) fclose(inf);
    }
    else
    {
        char ** vs = malloc(sizeof(char *) * (otk+1));
        
        int c = -1;

        do
        {   
            ++c;
            char * tmp = NULL;
            read = getline(&tmp,&line_sz,stdin);
            vs[p] = tmp;
            if(++p==otk+1) p = 0;
        }while(read!=-1);

        if(c >= otk) 
        {
            for(int i=p;i<otk+1;++i) printf("%s",vs[i]);
            for(int i=0;i<p-1;++i) printf("%s",vs[i]);        
        }
        else for(int i=0;i<p;++i) printf("%s",vs[i]);        

        free(vs);
    }
    
    
    err_check("tail");
    
    return 0;
}