#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#define	ERRFILEREAD	-1
#define	ERRFILEWRITE	-2
#define	ERRFILESEEK	-3

int addlinenum(int ifd, int ofd);

int main(int argc, char **argv)
{
	int ifd, iret;	// Variables for input file
	int ofd, oret;	// Variables for output file
	int ret;
	char err1[64] = "Need More Argument!\n";
	char err2[64] = "Too Many Arguments!\n";
	char err3[64] = "Input file is not exist!\n";
	char err4[64] = "Output file has problem!\n";
	char err5[64] = "Error while inserting number\n";
	char err6[64] = "Error while closing input file\n";
	char err7[64] = "Error while closing output file\n";

	if (argc < 3) {		// When there are not enough variables
		write(STDOUT_FILENO, err1, 20);
		exit(1);
	} else if (argc > 3) {	// When there are so many variables
		write(STDOUT_FILENO, err2, 20);
		exit(1);
	} else {
		/* Open input file */
		ifd = open(argv[1], O_RDONLY);
		if (ifd < 0) {
			write(STDOUT_FILENO, err3, 25);
			exit(1);
		}

		/* Create and open output file */
		ofd = open(argv[2], O_RDWR | O_CREAT,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
		if (ofd < 0) {
			write(STDOUT_FILENO, err4, 25);
			exit(1);
		}
		
		/* Add line number to output file */
		ret = addlinenum(ifd, ofd);
		if (ret < 0) {
			write(STDOUT_FILENO, err5, 29);
			exit(1);
		}

		/* Close input file */
		iret = close(ifd);
		if (iret < 0) {
			write(STDOUT_FILENO, err6, 31);
			exit(1);
		}

		/* Close output file */
		oret = close(ofd);
		if (oret < 0) {
			write(STDOUT_FILENO, err7, 32);
			exit(1);
		}
	}

	exit(0);
}

const int max_v = 1e2+7;
long long line = 512;
long long curline = 0;
int pad = 3;

char * mkline(int idx)
{
    char * tmp = malloc(sizeof(char)*pad);

    tmp[pad-1] = ' ';
    tmp[pad-2] = '|';
    tmp[pad-3] = ' ';

    for(int i=pad-4;i>=0;--i)
    {
        tmp[i] = (idx%10) +'0';
        idx/=10;
    }

    for(int i=0;i<pad;++i)
    {
        if(tmp[i]!='0') break;
        else tmp[i] = ' ';
    }

    return tmp;
}

int addlinenum(int ifd, int ofd)
{

    char ** str = malloc(sizeof(char*)*line);
    int * stridx = malloc(sizeof(int)*line);
    int * isline = malloc(sizeof(int)*line);
    char buf[max_v];
    int bl;
    int realline=0;


    do
    {
        bl = read(ifd,buf,sizeof(buf));
        
        char * curstr = (char*)malloc(sizeof(char)*(max_v));
        int spos=0;
        
        for(int i = 0;i<bl;++i)
        {
            curstr[spos++] = buf[i];

            if(buf[i]=='\n'||i==bl-1)
            {  
                realline += (buf[i]=='\n');
                if(buf[i]=='\n') isline[curline] = realline;
                else isline[curline] = 0;
                stridx[curline] = spos;
                str[curline++] = curstr;

                if(curline == line) 
                {
                    line *=2;
                    str = realloc(str,sizeof(char*)*line);
                    stridx = realloc(stridx,sizeof(int)*line);
                    isline = realloc(isline,sizeof(int)*line);
                }
                spos = 0;
                curstr = (char*)malloc(sizeof(char)*(max_v));
            }
        }

    }while(bl==max_v);

    while(realline)
    {
        ++pad;
        realline/=10;
    }

    int ck = 1;
    int r= 1;

    for(int i=0 ; i<curline;++i)
    {
        if(ck) write(ofd,mkline(r++),pad);            
        write(ofd,str[i],stridx[i]);
        ck = !!isline[i];
    }

    return 0;
}
