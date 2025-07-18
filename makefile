ifeq ($(OS),Windows_NT)
    CC = gcc
    RM = del /Q
    MKDIR = mkdir
    OBJ_EXT = .obj
    EXE_EXT = .exe
    PATH_SEP = \\
else
    CC ?= gcc
    RM = rm -rf
    MKDIR = mkdir -p
    OBJ_EXT = .o
    EXE_EXT =
    PATH_SEP = /
endif

CFLAGS = -Wall -Wextra -Iinclude -std=c11
LDFLAGS = 

SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.c) \
      $(wildcard $(SRC_DIR)/lexer/*.c) \
      $(wildcard $(SRC_DIR)/parser/*.c) \
      $(wildcard $(SRC_DIR)/interpretator/*.c) \
      $(wildcard $(SRC_DIR)/data_structures/*.c)

OBJ_DIR = obj
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%$(OBJ_EXT),$(SRC))

TARGET = dorl$(EXE_EXT)

fixpath = $(subst /,$(PATH_SEP),$1)

.PHONY: all clean directories

all: directories $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) $(call fixpath,$^) -o $(call fixpath,$@)

$(OBJ_DIR)/%$(OBJ_EXT): $(SRC_DIR)/%.c
	@$(MKDIR) $(call fixpath,$(@D))
	$(CC) $(CFLAGS) -c $(call fixpath,$<) -o $(call fixpath,$@)

directories:
	@$(MKDIR) $(call fixpath,$(OBJ_DIR))
	@$(MKDIR) $(call fixpath,$(OBJ_DIR)/lexer)
	@$(MKDIR) $(call fixpath,$(OBJ_DIR)/parser)
	@$(MKDIR) $(call fixpath,$(OBJ_DIR)/interpretator)
	@$(MKDIR) $(call fixpath,$(OBJ_DIR)/data_structures)

clean:
	$(RM) $(call fixpath,$(OBJ_DIR))
	$(RM) $(call fixpath,$(TARGET))