#include "definition.h"
#include "errhandler.h"
#include "dirent.h"

char src[512];

int main(int argc,char * argv[])
{
    set_default();
    if(argc==1) fprintf(stderr,"mv: missing file operand\n");
    else if(argc==2) fprintf(stderr,"mv: missing destination file operand after %s\n",argv[1]);   
    else
    {
 
        int d = -1;
        while(argv[1][++d]);
        while(d>0 && argv[1][--d]!='/');
        if(argv[1][d]=='/') ++d;
        for(int i=0;argv[1][d];++i) src[i] = argv[1][d++];

        int c = -1;
        while(argv[2][++c]);
        int ck = (argv[2][--c] == '/');

        if(ck) strcat(argv[2],src);
        else
        {            
            DIR * dcheck = opendir(argv[2]);

            if(errno == EACCES)
            {
                err_check("mv");
                return 0;
            }

            if(dcheck)
            {                
                strcat(argv[2],"/");strcat(argv[2],src);
            }            
            if(dcheck) closedir(dcheck);
        }
        
         
        set_default();          
        rename(argv[1],argv[2]);
    }
    
    err_check("mv");
    return 0;
}