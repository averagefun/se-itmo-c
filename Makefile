ASM=nasm
ASMFLAGS=-f elf64
LD=ld

SRC=src
OBJ=obj

$(OBJ)/%.o: $(SRC)/%.asm
	$(ASM) $(ASMFLAGS) $^ -o $@

main: $(OBJ)/%.o
	$(LD) $(OBJ)/*.o -o $@

