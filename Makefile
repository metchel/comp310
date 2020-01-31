all: build clean

build:
	@echo "Compiling..."
	gcc -c shell.c interpreter.c shellmemory.c
	@echo "Linking..."
	gcc -o mysh shell.o interpreter.o shellmemory.o

clean:
	@echo "Cleaning up object files..."
	rm *.o
