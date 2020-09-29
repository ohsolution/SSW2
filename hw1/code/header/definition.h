#ifndef _DEFINITON_H
#define _DEFINITON_H 1

/* Standard Libary header */
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>


/* boolean Datatype */
typedef enum { false, true } bool;

/* Standard Steram */
#define STDIN 0
#define STDOUT 1
#define STDERR 2

/* Input Type */

typedef int Qtype;
#define SINGLEWORD 1
#define MULTWORD 2
#define SENTENCE 3
#define REGULAREXP 4
#define WRONGQUERY 0

typedef struct _query
{
    char ** word;
    int wordnum;
    Qtype type;
} query;





#endif /* definition.h */
