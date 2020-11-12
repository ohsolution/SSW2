#ifndef _CVECTOR_H
#define _CVECTOR_H 1

#include <stdlib.h>
#include <stdio.h>

typedef struct _node
{
    int v;    
}node;


typedef struct _cvector
{
    int csz;
    int tsz;
    int * arr;
}cvector;

void cv_init(cvector * _cv,int _sz);
void cv_push(cvector * _cv,int _data);
int cv_pop(cvector * _cv);
int cv_size(cvector *_cv);
int cv_back(cvector * _cv);
void cv_clear(cvector *_cv);
void cv_print(cvector *_cv);

#endif /* cvector.h */