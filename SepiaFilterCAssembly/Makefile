NAME := image-transformer

RM = rm -rf
MKDIR = mkdir -p

# c
CC = gcc
LINKER = $(CC)

# asm
ASM = nasm
ASMFLAGS = -f elf64 -g -F dwarf

SOL_DIR = solution

BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

SRC_DIR = $(SOL_DIR)/src
HEAD_DIR = $(SOL_DIR)/include

CFLAGS = $(strip $(file < $(SOL_DIR)/compile_flags.txt)) -I $(HEAD_DIR)

TARGET  := $(BUILD_DIR)/$(NAME)

SOURCES_C := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*/*.c)
OBJECTS_C := $(SOURCES_C:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DIRS_C := $(sort $(dir $(OBJECTS_C) $(TARGET)))

SOURCES_ASM := $(wildcard $(SRC_DIR)/*.asm) $(wildcard $(SRC_DIR)/*/*.asm)
OBJECTS_ASM := $(SOURCES_ASM:$(SRC_DIR)/%.asm=$(OBJ_DIR)/%.o)
DIRS_ASM := $(sort $(dir $(OBJECTS_ASM) $(TARGET)))

OBJECTS := $(OBJECTS_C) $(OBJECTS_ASM)
DIRS := $(DIRS_C) $(DIRS_ASM)

### Rules and targets

.PHONY: all build-main clean

all: build-main

$(TARGET): $(OBJECTS) | $(DIRS)
	$(LINKER) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm | $(DIRS_ASM)
	$(ASM) $(ASMFLAGS) $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(DIRS_C)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $@.d -MP

build-main: $(TARGET)

clean:
	$(RM) $(OBJ_DIR) $(BUILD_DIR)

$(sort $(DIRS)):
	$(MKDIR) $@
