#include "../header/definition.h"

char const err_a[64] = "Need More Argument!\n";
char const err_b[64] = "Too many Arguments!\n";
char const err_c[64] = "Input file is not exist!\n";
char const err_d[64] = "reallocating error!\n";

char const endl[1] = "\n";
char const blank[1] = " ";

size_t * offsetsz;
Qtype * tyline;
long long line = 1024;
int max_v = 1024; 
long long curline=1;
int IFD;
char output[100007];
int osz = 0;


