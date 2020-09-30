#ifndef _IO_H
#define _IO_H 1

#include "../definition.h"

void secure_open(int __argc, char ** __argv);

void secure_write(int __fd,char * __str,size_t __sz);

void save_offset();

query ConsoleInput(void);



#endif /* IO.h */
