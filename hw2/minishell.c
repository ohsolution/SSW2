//-----------------------------------------------------------
//
// SSE2033 : System Software Experiment 2 (Fall 2020)
//
// Skeleton Code for PA#2
//
// CSI, Sungkyunkwan University
//
//-----------------------------------------------------------


/* $ begin shellmain */
#define MAXARGS   128
#define MAXLINE	  256
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* function prototypes */
void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv); 

int main() 
{
    char cmdline[MAXLINE]; /* Command line */
    char *ret;

    while (1) {
	/* Read */
	printf("mini> ");                   
	ret = fgets(cmdline, MAXLINE, stdin); 
	if (feof(stdin) || ret == NULL)
	    exit(0);

	/* Evaluate */
	eval(cmdline);
    } 
}
/* $end shellmain */
  
/*
 * $ begin eval
 * eval - Evaluate a command line
 */
void eval(char *cmdline) 
{
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in bg or fg? */
    pid_t pid;           /* Process id */
    
    strcpy(buf, cmdline);
    bg = parseline(buf, argv);

    /* Ignore empty lines */
    if (argv[0] == NULL)
	return;

    if (!builtin_command(argv)) { 
	/* Child runs user job */
	if ((pid = fork()) == 0) {
	    if (execv(argv[0], argv) < 0) {
		fprintf(stderr,"%s: Command not found.\n", argv[0]);
		exit(0);
	    }
	}

	/* Parent waits for foreground job to terminate */
	if (!bg) {
	    int status;
	    if (waitpid(pid, &status, 0) < 0)
			printf("waitfg: waitpid error");
	}
	else
	    printf("%d %s", pid, cmdline);
    }
    return;
}

/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv) 
{
    
    /* Insert your code */

    return 0;
}
/* $end eval */

/* 
 *  $ begin parseline
 *  parseline - Parse the command line and build the argv array
 */
int parseline(char *buf, char **argv) 
{
    char *delim;         /* Points to first space delimiter */
    int argc;            /* Number of args */
    int bg;              /* Background job? */

    buf[strlen(buf)-1] = ' ';  /* Replace trailing '\n' with space */

    /* Ignore leading spaces */
    while (*buf && (*buf == ' '))
	buf++;

    /* Build the argv list */
    argc = 0;
    while ((delim = strchr(buf, ' '))) {
	argv[argc++] = buf;
	*delim = '\0';
	buf = delim + 1;

	/* Ignore spaces */
	while (*buf && (*buf == ' '))
	       buf++;
    }
    argv[argc] = NULL;
    
    /* Ignore blank line */
    if (argc == 0)
	return 1;

    /* Should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0)
	argv[--argc] = NULL;

    return bg;
}
/* $end parseline */


