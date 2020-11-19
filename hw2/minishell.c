//-----------------------------------------------------------
//
// SSE2033 : System Software Experiment 2 (Fall 2020)
//
// Skeleton Code for PA#2
//
// CSI, Sungkyunkwan University
//
//-----------------------------------------------------------

#include "definition.h"
#include "table.h"
#include "errhandler.h"
#include "cvector.h"

/* function prototypes */
void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv); 
int invaild_checker(char ** argv);
int parsecommand(char **argv,cvector * vsz,cvector * vtp);

char cwd[1024];

pid_t ppid;           
pid_t mpgid;

void ign_handler(int sig)
{
    if(getppid() == mpgid) killpg(getpgid(getpid()),SIGKILL);
    
    exit(0);
}

void killing_zombie_handler(int sig)
{
    signal(SIGTTOU,SIG_IGN);
    tcsetpgrp(STDOUT_FILENO,mpgid);
    int st = 0;

    pid_t zpid = waitpid(-1,&st,WNOHANG|WUNTRACED);
    if(WIFSTOPPED(st)) killpg(getpgid(zpid),SIGKILL);
    
    signal(SIGTTOU,SIG_DFL);
}

int main() 
{
    char cmdline[MAXLINE]; /* Command line */
    char *ret;
    getcwd(cwd,sizeof(cwd)); // get bin file location

    mpgid = getpgid(getpid());
    
    signal(SIGTSTP,SIG_IGN);
    signal(SIGINT,SIG_IGN);
    signal(SIGCHLD,killing_zombie_handler);
    
    while (1) 
    {  
        int st=0;

        printf("mini> ");                   
        ret = fgets(cmdline, MAXLINE, stdin); 
        if (feof(stdin) || ret == NULL) exit(0);
                
        pid_t zpid = waitpid(-1,&st,WNOHANG|WUNTRACED);
        if(WIFSTOPPED(st)) killpg(getpgid(zpid),SIGKILL);
        
        eval(cmdline);    
        
        zpid = waitpid(-1,&st,WNOHANG|WUNTRACED);
        if(WIFSTOPPED(st)) killpg(getpgid(zpid),SIGKILL);    
        
    } 
}

void eval(char *cmdline) 
{
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in bg or fg? */
    
    pid_t pid;

    cvector vsz,vtp;
    cv_init(&vsz,4); cv_init(&vtp,4);

    strcpy(buf, cmdline);
    bg = parseline(buf, argv);

    /* Ignore empty lines */
    if (argv[0] == NULL) return;

    if(invaild_checker(argv) || parsecommand(argv,&vsz,&vtp)) 
    {
        fprintf(stderr,"mini: command not found\n");
        cv_clear(&vsz);
        cv_clear(&vtp); 
        return;
    }

    int cn = cv_size(&vsz);    
    int c=0;
    int fd[2];
    int bfd=0;

    if(builtin_command(argv))
    {
        cv_clear(&vsz);
        cv_clear(&vtp);
        return;
    }    

    if((ppid=fork()))
    {
        cv_clear(&vsz);
        cv_clear(&vtp);    
        if(!bg) waitpid(ppid,NULL,0);
        return;
    }
    
    
    setpgrp();
    
    if(!bg) // swap session ground 
    {
        signal(SIGTTOU,SIG_IGN);
        tcsetpgrp(STDOUT_FILENO,getpgid(getpid()));
    }

    signal(SIGTSTP,SIG_DFL);
    signal(SIGINT,ign_handler);

    for(int i=0;i<cn;++i,++c)
    {
        int saz = vsz.arr[i] - (vtp.arr[i]/3) * 2;
        int pc = c;

        char ** command = malloc(sizeof(char*) * (saz+1));
        
        char * f1 = NULL;
        char * f2 = NULL;

        for(int j = 0; c-pc< vsz.arr[i]; ++j)
        {
            int x = cmap(argv[c]);
            if(x >= 0) command[j] = argv[c++];
            else 
            {
                ++c;
                if(f1) f2 = argv[c++];
                else f1 = argv[c++];
                --j;
            }
        }

        command[saz] = NULL;

        pipe(fd);

        pid = fork();

        if (!pid) 
        {
            char path[512];
            
            dup2(bfd,0);

            if(i != cn-1) dup2(fd[1],STDOUT_FILENO);                                
                                    
            close(fd[0]);

            int tp = vtp.arr[i];

            if(tp == 4 || tp == 5) // output redirection
            {
                tp -=4;                
                int ofd = open(f1,(tp*O_APPEND)|O_WRONLY| O_CREAT,0644);                
                dup2(ofd,STDOUT_FILENO);
            }
            else if(tp==3) // input redirection
            {
                int ifd = open(f1,O_RDONLY);
                if(ifd==-1) 
                {
                    fprintf(stderr,"mini: No such file or directory\n");
                    exit(1);
                }
                dup2(ifd,STDIN_FILENO);                
            }
            else if(tp==7) // both redirection
            {
                int ifd = open(f1,O_RDONLY);
                int ofd = open(f2,O_WRONLY| O_CREAT,0644);

                dup2(ifd,STDIN_FILENO);
                dup2(ofd,STDOUT_FILENO);
            }
            

            int ctp = cmap(command[0]);

            set_default();

            if(ctp >10) // exec bin
            {
                if(ctp == 15) // awk handling
                {
                    for(int i=0;i<saz;++i) 
                    {
                        int idx = -1;
                        while(command[i][++idx]) if(command[i][idx]=='\'') command[i][idx] = '\"';                     
                    }
                }

                if(ctp >12) sprintf(path,"/usr/bin/%s",command[0]);    
                else sprintf(path,"/bin/%s",command[0]);    
                                                                             
                execv(path,command);                            
            }
            else if(ctp==10) // exec path
            {
                sprintf(path,"%s",command[0]);
                execv(path,command);                
            }
            else // exec implement
            {                
                sprintf(path,"%s/%s",cwd,command[0]);                
                execv(path,command);
            }

            err_check(command[0]);
            exit(0);            
        }
        else
        {
            waitpid(pid,NULL,0);          
            close(fd[1]);
            bfd = fd[0];            
        }

        free(command);                    
    }
        
    cv_clear(&vsz);
    cv_clear(&vtp);

    if(!bg) tcsetpgrp(STDOUT_FILENO,mpgid);
    
    exit(0);
}

int builtin_command(char **argv) 
{
    int tmp = cmap(argv[0]);
    
    if(tmp == 1) //EXIT
    {
        set_default();
        fprintf(stderr,"exit\n");
        int n = 0;
        if(argv[1]) n = atoi(argv[1]); 
        exit(n);
        err_check("exit");        
    }
    else if(tmp == 2) // CD
    {
        set_default();

        chdir(argv[1]);
        err_check("cd");         

        return 1;
    }

    return 0;
}

int parseline(char *buf, char **argv) 
{
    char *delim;         /* Points to first space delimiter */
    int argc;            /* Number of args */
    int bg;              /* Background job? */

    buf[strlen(buf)-1] = ' ';  /* Replace trailing '\n' with space */

    /* Ignore leading spaces */
    while (*buf && (*buf == ' ')) buf++;

    /* Build the argv list */
    argc = 0;
    while ((delim = strchr(buf, ' '))) 
    {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;

        /* Ignore spaces */
        while (*buf && (*buf == ' ')) buf++;
    }
    argv[argc] = NULL;
    
    /* Ignore blank line */
    if (argc == 0) return 1;

    /* Should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0) argv[--argc] = NULL;

    return bg;
}

int invaild_checker(char ** argv)
{
    int c = -1;
    while(argv[++c]);

    if(cmap(argv[0]) <= 0 || cmap(argv[c-1]) < -1) return 1; // check first,last is vaild

    for(int i=0; i< c-1; ++i)
    {
        if(cmap(argv[i]) == -2 && cmap(argv[i+1]) <= 0) return 1; // check pipe nxt is command
        if(cmap(argv[i]) < 0 && cmap(argv[i+1]) < 0) return 1;   
    }

    return 0; // vaild
}


/* tokenizing command by command and save each token size and type */
int parsecommand(char **argv,cvector * vsz,cvector * vtp) 
{
    int sz = 0;
    int tp = 0;

    for(int i=0;argv[i];++i,++sz)
    {
        int t = cmap(argv[i]);

        if(t==-2)
        {
            cv_push(vsz,sz);
            cv_push(vtp,tp);

            if(tp && tp!=3 && tp!=4 && tp!=5 && tp !=7) return 1;

            sz=-1;
            tp =0;
        }
        else if(t<-2) tp -= t;
    }

    cv_push(vsz,sz);
    cv_push(vtp,tp);

    if(tp && tp!=3 && tp!=4 && tp!=5 && tp !=7) return 1;

    return 0;
}