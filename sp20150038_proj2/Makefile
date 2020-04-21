all: 20150038.out

20150038.out: 20150038.c Basic.c Memory.c Opcode.c Assemble.c
			gcc -Wall -g -o 20150038.out Basic.c Memory.c  20150038.c Opcode.c Assemble.c -lm

clean:
			rm 20150038.out *.lst *.obj
