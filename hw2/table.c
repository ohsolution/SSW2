#include "table.h"

int cmap(char * str)
{
    /* keyword -1 ~ -5 */ 

    if(!strcmp(str,"&")) return -1;
    else if(!strcmp(str,"|")) return -2;
    else if(!strcmp(str,"<")) return -3;
    else if(!strcmp(str,">")) return -4;
    else if(!strcmp(str,">>")) return -5;
    

    /* bulitin command 1~2 */
    
    else if(!strcmp(str,"exit")) return 1;
    else if(!strcmp(str,"cd")) return 2;

    /* imple command 3 ~ 9*/

    else if(!strcmp(str,"head")) return 3;
    else if(!strcmp(str,"tail")) return 4;
    else if(!strcmp(str,"cat")) return 5;
    else if(!strcmp(str,"cp")) return 6;
    else if(!strcmp(str,"mv")) return 7;
    else if(!strcmp(str,"rm")) return 8;
    else if(!strcmp(str,"pwd")) return 9;

    else if(strlen(str) >= 2 && str[0] == '.' && str[1] == '/') return 10; // path executable

    /* bin command 11 ~ 16*/

    else if(!strcmp(str,"ls")) return 11;
    else if(!strcmp(str,"grep")) return 12;
    else if(!strcmp(str,"man")) return 13;     
    else if(!strcmp(str,"sort")) return 14;
    else if(!strcmp(str,"awk")) return 15;
    else if(!strcmp(str,"bc")) return 16;

    else return 0; // filename || option || argument || invalid command    
}
