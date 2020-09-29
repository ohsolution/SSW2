#ifndef _GLOBALDATA_H
#define _GLOBALDATA_H 1

#include "definition.h"

/* Error Message */
extern const char err_a[64]; 
extern const char err_b[64]; 
extern const char err_c[64]; 


/* for padding */
extern const char padding;


/* offset saving array */
extern size_t * offsetsz;
extern Qtype * tyline;
extern long long line;
extern int max_v;
extern long long curline;

#endif /* GlobalData.h */
