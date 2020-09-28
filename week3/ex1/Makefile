# Makefile

TARGET=PlusMinus
COMPILER=gcc
OBJECTS=main.o plus.o minus.o

$(TARGET): $(OBJECTS)
	$(COMPILER) -o $(TARGET) $^

main.o: main.c
	$(COMPILER) -c $<

plus.o: plus.c
	$(COMPILER) -c $<

minus.o: minus.c
	$(COMPILER) -c $<

clean:
	rm $(OBJECTS) $(TARGET)


