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

ifdef OS
    RM = del /Q
    FixPath = $(subst /,\,$1)
else
    ifeq ($(shell uname), Linux)
        RM = rm -f
        FixPath = $1
    endif
endif

clean:
	$(RM) $(TARGET) *.exe *.asm *.hack *.bin
	$(RM) $(call FixPath,test/*.asm)
	$(RM) $(call FixPath,test/*.hack)
	$(RM) $(call FixPath,test/*.bin)
