# Makefile

TARGET=pa2 head cat mv rm pwd cp tail

all: $(TARGET)

pa2:
	gcc -o pa2 minishell.c table.c errhandler.c cvector.c

head:
	gcc -o head head.c errhandler.c
cat:
	gcc -o cat cat.c errhandler.c
mv:
	gcc -o mv mv.c errhandler.c
rm:
	gcc -o rm rm.c errhandler.c
pwd:
	gcc -o pwd pwd.c errhandler.c
cp:
	gcc -o cp cp.c errhandler.c
tail:
	gcc -o tail tail.c errhandler.c cvector.c

clean:
	rm $(TARGET)





