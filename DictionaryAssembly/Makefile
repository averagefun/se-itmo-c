ASM=nasm
ASMFLAGS=-f elf64
LD=ld

SRC=src
OBJ=obj

PROGRAM=program

$(PROGRAM): $(OBJ)/main.o $(OBJ)/lib.o $(OBJ)/dict.o 
	$(LD) $^ -o $@

$(OBJ)/%.o: $(SRC)/%.asm
	mkdir -p $(OBJ)
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf $(OBJ) $(PROGRAM)

.PHONY: clean
