#include "definition.h"
#include "errhandler.h"

void set_default(void)
{
    errno = 0;
    return;
}

void err_check(char * str)
{
    if(!errno) return;

    switch (errno)
    {
    case EACCES:
        fprintf(stderr,"%s: Permission denied\n",str);
        break;

    case EISDIR:
        fprintf(stderr,"%s: Is a directory\n",str);
        break;

    case ENOENT:
        fprintf(stderr,"%s: No such file or directory\n",str);
        break;

    case ENOTDIR:
        fprintf(stderr,"%s: Not a directory\n",str);
        break;            

    case EPERM:
        fprintf(stderr,"%s: Operation not permitted\n",str);
        break;

    default:
        printf("%s: Error occurred: <%d>\n",str,errno);
        break;
    }

    return;
}   