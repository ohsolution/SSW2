#include "definition.h"
#include "errhandler.h"

char pwd[1024];

int main(void)
{
    set_default();
    getcwd(pwd,sizeof(pwd));
    printf("%s\n",pwd);
    err_check("pwd");
    return 0;
}