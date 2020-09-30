# Makefile

TARGET=pa1
D=source/
F=source/function/
OBJECTS= $(D)hw.c $(D)GlobalData.c $(F)IO.c #$(F)parser $(F)search
OBJ=hw.o GlobalData.o IO.o #parser.o search.o

all: $(TARGET)

$(TARGET): $(OBJ)
	gcc -o $(TARGET) $(OBJ)

$(OBJ): $(OBJECTS)
	gcc -c $^

clean:
	rm $(OBJ) $(TARGET)





