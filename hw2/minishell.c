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

int main() 
{
    char cmdline[MAXLINE]; /* Command line */
    char *ret;
    getcwd(cwd,sizeof(cwd));

    while (1) 
    {
        printf("mini> ");                   
        ret = fgets(cmdline, MAXLINE, stdin); 
        if (feof(stdin) || ret == NULL) exit(0);

        eval(cmdline);
    } 
}

void eval(char *cmdline) 
{
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in bg or fg? */
    pid_t pid;           /* Process id */
    cvector vsz,vtp;
    cv_init(&vsz,4); cv_init(&vtp,4);

    strcpy(buf, cmdline);
    bg = parseline(buf, argv);

    /* Ignore empty lines */
    if (argv[0] == NULL) return;

    if(invaild_checker(argv) || parsecommand(argv,&vsz,&vtp)) 
    {
        fprintf(stderr,"mini: command not found\n");
        goto EXIT;
    }

    //cv_print(&vsz);
    //cv_print(&vtp);

    int cn = cv_size(&vsz);
    int c=0;

    if(builtin_command(argv)) goto EXIT;

    int fd[MAXARGS][2];

    for(int i=0;i<cn-1;++i,++c)
    {
        char ** cargv = malloc(sizeof(char*) * vsz.arr[i]);
        
        for(int j = 0; j< vsz.arr[i] ; ++j) cargv[j] = argv[c++];

        if (!builtin_command(cargv)) 
        { 
            /* Child runs user job */
            if ((pid = fork()) == 0) 
            {
                if (execv(argv[0], argv) < 0) 
                {
                    //fprintf(stderr,"mini: command not found.\n");
                    exit(0);
                }
            }
            
            int status;
            if (waitpid(pid, &status, 0) < 0) printf("waitfg: waitpid error");	        
        }

        free(cargv);
    }

    if (!builtin_command(argv)) 
    { 
        /* Child runs user job */
        if ((pid = fork()) == 0) 
        {
            if (execv(argv[0], argv) < 0) 
            {
                //fprintf(stderr,"mini: command not found.\n");
                exit(0);
            }
        }

        /* Parent waits for foreground job to terminate */
        if (!bg) 
        {
            int status;
            if (waitpid(pid, &status, 0) < 0) printf("waitfg: waitpid error");	
        }
        else printf("%d %s", pid, cmdline);
    }
    
    EXIT:
    cv_clear(&vsz);
    cv_clear(&vtp);



    return;
}

int builtin_command(char **argv) 
{
    int tmp = cmap(argv[0]);
    
    if(tmp == 1) //EXIT
    {
        fprintf(stderr,"exit\n");
        int n = 0;
        if(argv[1]) n = atoi(argv[1]); 
        exit(n);        
    }
    else if(tmp == 2) // CD
    {
        set_default();

        chdir(argv[1]);

        getcwd(cwd,sizeof(cwd)); // for check
        printf("%s\n",cwd); // for check

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

    if(cmap(argv[0]) <= 0 || cmap(argv[c-1]) < -1) return 1;

    for(int i=0; i< c-1; ++i)
    {
        if(cmap(argv[i]) == -2 && cmap(argv[i+1]) <= 0) return 1;
        if(cmap(argv[i]) < 0 && cmap(argv[i+1]) < 0) return 1;
    }

    return 0;
}

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