CC := gcc -std=c99
CPPC := g++ -std=c++11
TARGET = asm vm

all: $(TARGET)

asm: asm.cpp
	$(CPPC) $^ -o $@

vm: vm.c
	$(CC) $^ -o $@

clean:
	del *.exe *.hack *.bin
