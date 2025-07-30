# ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
# ┃       🛠️ C PROJECT MAKEFILE         ┃
# ┃      With Optional Static Library   ┃
# ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

# Make silence cleaner
MAKEFLAGS += --no-print-directory

# ────────────────────────────────────────────────
# 🔧 Project Configuration
# ────────────────────────────────────────────────

EXEC = 2D_Engine		# 🟩 Name of your final executable (you can change this)

CC = gcc
CFLAGS = -Wall -Wextra -Werror		# 🛡️ Recommended safety flags

RM = rm -rf			# 🚮 Command to remove files/folders

# ────────────────────────────────────────────────
# 📁 Directory Layout (You may change these)
# ────────────────────────────────────────────────

# Your C source files
SRC_DIR = src
# Compiled object files
OBJ_DIR = obj
# Final compiled binary will go here
BIN_DIR = bin
# Your project's header files
INC_DIR = include

# ─────────────────────────────────────────────────────────────
# 📦 Optional External Library Setup
# Set the `USE_EXTLIB` flag when running make to enable this
# e.g. make USE_EXTLIB=1
# ─────────────────────────────────────────────────────────────

# 📁 Folder name of the external library
EXTLIB_DIR = your_lib_dir_name
# 📁 Location of its .h files
EXTLIB_INC = $(EXTLIB_DIR)/include
# 📦 Path to compiled .a static library
EXTLIB_LIB = $(EXTLIB_DIR)/yourlib.a

# 🔘 Default to disabled unless specified
USE_EXTLIB ?=0

# ────────────────────────────────────────────────
# 🔍 File Resolution
# ────────────────────────────────────────────────

# All .c source files
SRC = $(shell find $(SRC_DIR) -type f -name "*.c")

# Create list of corresponding .o files
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# ────────────────────────────────────────────────
# 🧠 Include & Linker Flags
# ────────────────────────────────────────────────

# Add your own headers
INCLUDES = -I$(INC_DIR)


ifeq ($(USE_EXTLIB), 1)

# Add external headers
    INCLUDES += -I$(EXTLIB_INC)
# Allow conditional compilation in code
    CFLAGS += -DUSE_EXTLIB
# Link with external static lib
    LDFLAGS = -L$(EXTLIB_DIR) -lflux
# Build target for the lib
    LIB_TARGET = $(EXTLIB_LIB)
else
    LDFLAGS = -lX11
    LIB_TARGET =
endif

# ────────────────────────────────────────────────
# 🧱 Build Targets
# ────────────────────────────────────────────────

# 🧱 Main entry point
all: $(LIB_TARGET) $(BIN_DIR)/$(EXEC)

# 📦 External static lib build step (optional)
ifeq ($(USE_EXTLIB), 1)
$(EXTLIB_LIB):
	@echo "📦 Building external static library..."
	@$(MAKE) -C $(EXTLIB_DIR)
endif

# 🧩 Object file compilation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "🔧 Compiling $< -> $@"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# 🔗 Final linking stage
$(BIN_DIR)/$(EXEC): $(OBJ)
	@if [ -z "$(OBJ)" ]; then \
		echo "❌ No object files found! Check your source directory."; \
		exit 1; \
	fi
	@echo "🔗 Linking into final executable..."
	@mkdir -p $(BIN_DIR)
	@$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@
	@echo "✅ Built executable: $@"

# ────────────────────────────────────────────────
# 🧹 Clean-Up Commands
# ────────────────────────────────────────────────

# Delete object files
clean:
	@echo "🧼 Cleaning object files..."
	@$(RM) $(OBJ_DIR)
ifeq ($(USE_EXTLIB), 1)
	@$(MAKE) -C $(EXTLIB_DIR) clean
endif

# Delete everything
fclean: clean
	@echo "🧼 Cleaning binaries..."
	@$(RM) $(BIN_DIR)
ifeq ($(USE_EXTLIB), 1)
	@$(MAKE) -C $(EXTLIB_DIR) fclean
endif

# Rebuild everything
re: fclean all

# Prevent Makefile from trying to build these as files
.PHONY: all clean fclean re
