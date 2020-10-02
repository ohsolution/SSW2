#ifndef _GLOBALDATA_H
#define _GLOBALDATA_H 1

#include "definition.h"

/* Error Message */
extern const char err_a[64]; 
extern const char err_b[64]; 
extern const char err_c[64]; 
extern const char err_d[64];

/* for padding */
extern const char endl[1];
extern const char blank[1];

/* offset saving array */
extern size_t * offsetsz;
extern Qtype * tyline;
extern long long line;
extern int max_v;
extern long long curline;

/*file descriptor */
extern int IFD;

/*OUTPUT */

extern char output[100007];
extern int osz;

#endif /* GlobalData.h */
