# Makefile

TARGET=pa2 head cat mv rm pwd cp tail

all: $(TARGET)

pa2:
	gcc -o pa2 minishell.c table.c errhandler.c cvector.c

head:
	gcc -o head head.c errhandler.c
cat:
	gcc -o head cat.c errhandler.c
mv:
	gcc -o head mv.c errhandler.c
rm:
	gcc -o head rm.c errhandler.c
pwd:
	gcc -o head pwd.c errhandler.c
cp:
	gcc -o head cp.c errhandler.c
tail:
	gcc -o head tail.c errhandler.c cvector.c

clean:
	rm $(TARGET)





