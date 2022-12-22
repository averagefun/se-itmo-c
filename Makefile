NAME := image-transformer

RM = rm -rf
MKDIR = mkdir -p

CC = gcc
LINKER = $(CC)

SOL_DIR = solution

BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

SRC_DIR = $(SOL_DIR)/src
HEAD_DIR = $(SOL_DIR)/include

CFLAGS = $(strip $(file < $(SOL_DIR)/compile_flags.txt)) -I $(HEAD_DIR)

SOURCES += $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*/*.c)
TARGET  := $(BUILD_DIR)/$(NAME)

OBJECTS := $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DIRS := $(sort $(dir $(OBJECTS) $(TARGET)))

### Rules and targets

.PHONY: all build-main clean

all: build-main

$(TARGET): $(OBJECTS) | $(DIRS)
	$(LINKER) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(DIRS)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $@.d -MP

build-main: $(TARGET)

clean:
	$(RM) $(OBJ_DIR) $(BUILD_DIR)

$(sort $(DIRS)):
	$(MKDIR) $@
