#ifndef _IO_H
#define _IO_H 1

void secure_open(int * __fd,int __argc, char ** __argv);

void secure_write(int __fd,char * __str,size_t __sz);

void save_offset(int __fd);

char * ConsoleInput(void);



#endif /* IO.h */
