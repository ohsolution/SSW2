
cmdline
        : pipeline BACKGROUND?
        ;

pipeline
        : commands
        | pipeline PIPE commands
        ;

commands
        : command
        | command '<' FILENAME
        | command '>' FILENAME
        | command '>>' FILENAME
        | command '<' FILENAME '>' FILENAME
        ;

command
        : executable OPTION* ARGUMENT*
        | builtin_cmd ARGUMENT?
        ;

executable
        : LS
        | MAN
        | GREP
        | SORT
        | AWK
        | BC
        | HEAD
        | TAIL
        | CAT
        | CP
        | MV
        | RM
        | PWD
        | path
        ;

builtin_cmd
        : CD
        | EXIT
        ;

path
        : PATHNAME


BACKGROUND = '&';
PIPE = '|'
OPTION = ID
ARGUMENT = ID
PATHNAME = ID
ID = (~[ ])+