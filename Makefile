# ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
# ┃       🛠️ CROSS-PLATFORM MAKEFILE    ┃
# ┃   Linux / Windows / macOS Support   ┃
# ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

MAKEFLAGS += --no-print-directory

EXEC = 2D_Engine
CFLAGS = -Wall -Wextra -Werror
RM = rm -rf

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = include

EXTLIB_DIR = your_lib_dir_name
EXTLIB_INC = $(EXTLIB_DIR)/include
EXTLIB_LIB = $(EXTLIB_DIR)/yourlib.a
USE_EXTLIB ?=0

SRC_C = $(shell find $(SRC_DIR) -type f -name "*.c")
SRC_M = $(shell find $(SRC_DIR) -type f -name "*.m")
SRC = $(SRC_C) $(SRC_M)

OBJ_C = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_C))
OBJ_M = $(patsubst $(SRC_DIR)/%.m, $(OBJ_DIR)/%.o, $(SRC_M))
OBJ = $(OBJ_C) $(OBJ_M)

INCLUDES = -I$(INC_DIR)
ifeq ($(USE_EXTLIB), 1)
    INCLUDES += -I$(EXTLIB_INC)
    CFLAGS += -DUSE_EXTLIB
    LDFLAGS += -L$(EXTLIB_DIR) -lflux
    LIB_TARGET = $(EXTLIB_LIB)
else
    LIB_TARGET =
endif

# Default target
all: linux

linux: CC = gcc
linux: LDFLAGS += -lX11
linux: $(BIN_DIR)/$(EXEC)

windows: CC = x86_64-w64-mingw32-gcc
windows: LDFLAGS += -lgdi32 -luser32
windows: EXEC := $(EXEC).exe
windows: $(BIN_DIR)/$(EXEC)

macos: CC = clang
macos: OBJCFLAGS = -ObjC
macos: LDFLAGS += -framework Cocoa
macos: $(BIN_DIR)/$(EXEC)

all: $(LIB_TARGET) $(BIN_DIR)/$(EXEC)

ifeq ($(USE_EXTLIB), 1)
$(EXTLIB_LIB):
	@echo "📦 Building external static library..."
	@$(MAKE) -C $(EXTLIB_DIR)
endif

# Compile .c files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "🔧 Compiling $< -> $@"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compile .m (Objective-C) files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.m
	@echo "🔧 Compiling Objective-C $< -> $@"
	@mkdir -p $(dir $@)
	@clang $(OBJCFLAGS) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Final linking
$(BIN_DIR)/%: $(OBJ)
	@if [ -z "$(OBJ)" ]; then \
		echo "❌ No object files found! Check your source directory."; \
		exit 1; \
	fi
	@echo "🔗 Linking into final executable..."
	@mkdir -p $(BIN_DIR)
	@$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@
	@echo "✅ Built executable: $@"

clean:
	@echo "🧼 Cleaning object files..."
	@$(RM) $(OBJ_DIR)
ifeq ($(USE_EXTLIB), 1)
	@$(MAKE) -C $(EXTLIB_DIR) clean
endif

fclean: clean
	@echo "🧼 Cleaning binaries..."
	@$(RM) $(BIN_DIR)
ifeq ($(USE_EXTLIB), 1)
	@$(MAKE) -C $(EXTLIB_DIR) fclean
endif

re: fclean all

.PRECIOUS: $(OBJ)
.PHONY: all clean fclean re linux windows macos

