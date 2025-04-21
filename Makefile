CC := gcc
NAME := minishell

# Directories
PARN_DIR := .
SRC_DIR := $(PARN_DIR)/src
INCLUDE_DIR := $(PARN_DIR)/include
BUILD_DIR := $(PARN_DIR)/build
LIBFT_DIR := ./libft

# Compilation flags
CFLAGS := #-Wall -Wextra -Werror 

DEBUG_FLAGS := -g3 -O0 -fsanitize=address,undefined -DDEBUG
RELEASE_FLAGS := -O2 -flto -march=native -D_FORTIFY_SOURCE=2 -fstack-protector-strong

# Select mode (default to release)
MODE ?= release
ifeq ($(MODE),debug)
	CFLAGS += $(DEBUG_FLAGS)
else
	CFLAGS += $(RELEASE_FLAGS)
endif

# Source and object files
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))

INCLUDE := $(wildcard $(INCLUDE_DIR)/*.h)

# Libraries
LIBFT := $(LIBFT_DIR)/libft.a
LDFLAGS := -lreadline

all: builddir lib $(NAME)

$(NAME): $(OBJ) $(LIBFT) $(INCLUDE)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME) $(LDFLAGS)
	@echo "\033[1;32m[OK]\033[0m Compiled \033[1m$(NAME)\033[0m in \033[1m$(MODE)\033[0m mode"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "\033[1;34m[CC]\033[0m $<"

builddir:
	@mkdir -p $(BUILD_DIR)

lib:
	@$(MAKE) -C $(LIBFT_DIR)

clean:
	@$(MAKE) clean -C $(LIBFT_DIR)
	@rm -f $(OBJ)
	@echo "\033[1;33m[clean]\033[0m Object files removed"

fclean: clean
	@$(MAKE) fclean -C $(LIBFT_DIR)
	@rm -f $(NAME)
	@rm -rf $(BUILD_DIR)
	@echo "\033[1;31m[fclean]\033[0m Binary and build directory removed"

re: fclean all

.PHONY: all clean fclean re lib builddir
