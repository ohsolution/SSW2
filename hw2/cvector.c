#include "cvector.h"

void cv_init(cvector * cv,int sz)
{
    cv->arr = malloc(sizeof(int) * sz);
    cv->csz = 0;
    cv->tsz = sz;
}

void cv_push(cvector * cv,int data)
{
    if(!cv->tsz)
    {
        cv->tsz = 16;
        cv->arr = malloc(sizeof(int) * 16);
    }

    cv->arr[(cv->csz)++] = data;
    if(cv->csz == cv->tsz) 
    {
        cv->tsz <<=1;
        void * tmp = realloc(cv->arr,sizeof(int) * (cv->tsz));
        if(tmp) cv->arr = tmp;
    }
}

int cv_size(cvector * cv)
{
    return cv->csz;
}

int cv_back(cvector * cv)
{
    return cv->arr[cv->csz-1];
}

int cv_pop(cvector * cv)
{
    cv->csz -= 1;
    return cv->arr[(cv->csz)];
}

void cv_clear(cvector * cv)
{
    free(cv->arr);
    cv->csz = 0;
    cv->tsz = 0;
}

void cv_print(cvector * cv)
{
    for(int i=0;i<cv->csz;++i) printf("%d : %d\n",i,cv->arr[i]);
    return;
}