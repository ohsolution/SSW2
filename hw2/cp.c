#include "definition.h"
#include "errhandler.h"
#include "dirent.h"

char src[512];

int main(int argc,char * argv[])
{
    set_default();
    if(argc==1) fprintf(stderr,"cp: missing file operand\n");
    else if(argc==2) fprintf(stderr,"cp: missing destination file operand after %s\n",argv[1]);   
    else
    {

        int ifd,ofd,ril;
        ifd = ofd = -1;
        char buf[1024];

        set_default();

        ifd = open(argv[1],O_RDONLY);

        if(errno)
        {
            err_check("cp");
            close(ifd);
            return 0;
        }
 
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

            if(errno)
            {
                err_check("cp");
                return 0;
            }

            if(dcheck)
            {                
                strcat(argv[2],"/");strcat(argv[2],src);
            }            
            if(dcheck) closedir(dcheck);
        }
        
    
        if(ifd == -1) ;
        else 
        {
            ofd = open(argv[2],O_WRONLY|O_CREAT,0644);
            while((ril = read(ifd,buf,1024)) > 0) write(ofd,buf,ril);
        }

        if(ifd != -1) close(ifd);
        if(ofd !=-1) close(ofd);
    }
    
    err_check("cp");
    return 0;
}