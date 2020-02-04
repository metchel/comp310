all: build clean

build:
	@echo "Compiling..."
	gcc -c shell.c interpreter.c shellmemory.c kernel.c cpu.c pcb.c ram.c
	@echo "Linking..."
	gcc -o mykernel shell.o interpreter.o shellmemory.o kernel.o cpu.o pcb.o ram.o
	rm -r ../bin --force
	mkdir ../bin
	mv mykernel ../bin/
clean:
	@echo "Cleaning up object files..."
	rm *.o
