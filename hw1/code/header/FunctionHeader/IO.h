#ifndef _IO_H
#define _IO_H 1

#include "../definition.h"

void secure_open(int __argc, char ** __argv);

void save_offset();

query ConsoleInput(void);

void num_write(int __ln,int __index);

#endif /* IO.h */
