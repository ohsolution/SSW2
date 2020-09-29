#ifndef _SEARCH_H
#define _SEARCH_H 1

#include "../definition.h"

char * search_singleword(query __input);

char * search_multword(query __input);

char * search_sentence(query __input);

char * search_regularexp(query __input);




#endif /* search.h */
