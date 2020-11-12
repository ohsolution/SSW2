#include <stdio.h>
#include "cvector.h"


int main()
{
    cvector vi;
    cv_init(&vi,2);
    for(int i=0; i< 10 ; ++i) cv_push(&vi,i);
    for(int i=0; i< vi.csz;++i) printf("%d ",vi.arr[i]);
    cv_pop(&vi);cv_pop(&vi);
    printf("\n %d \n",cv_size(&vi));
    return 0;
}
