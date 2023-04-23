CC := gcc -std=c99
CPPC := g++ -std=c++11
TARGET = compiler asm vm

all: $(TARGET)

compiler: ir.c lexer.c compiler.c main.c
	$(CC) $^ -o $@

asm: asm.cpp
	$(CPPC) $^ -o $@

vm: vm.c
	$(CC) $^ -o $@

clean:
	del *.exe *.asm *.hack *.bin
	del .\test\*.asm .\test\*.hack .\test\*.bin
