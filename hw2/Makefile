# Makefile

TARGET=pa2
OBJECTS= minishell.c table.c errhandler.c cvector.c
OBJ=minishell.o table.o errhandler.o cvector.o

all: $(TARGET)

$(TARGET): $(OBJ)
	gcc -o $(TARGET) $(OBJ)

$(OBJ): $(OBJECTS)
	gcc -c $^

clean:
	rm $(OBJ) $(TARGET)





