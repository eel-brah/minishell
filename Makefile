CC := cc
CFLAGS := -Wall -Wextra -Werror -g -fsanitize=address

PARN_DIR := .
SRC_DIR := $(PARN_DIR)/src
INCLUDE_DIR := $(PARN_DIR)/include
BUILD_DIR := $(PARN_DIR)/build
LIBFTDIR := ./libft

INCLUDE := msh.h
INCLUDE := $(addprefix $(INCLUDE_DIR)/,$(INCLUDE))

SRC := msh.c exec.c prompt.c utils.c setup.c \
	builtins.c expander.c export.c get_next_line.c \
	parsing.c execution.c free.c \
	parsing_utils_0.c parsing_utils_1.c parsing_utils_2.c \
	parsing_utils_3.c ft_setenv.c
OBJ := $(SRC:%.c=$(BUILD_DIR)/%.o)

LIBFT := $(LIBFTDIR)/libft.a

NAME := minishell

all: lib $(NAME)


$(NAME): $(OBJ) $(LIBFT) $(INCLUDE)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME) -lreadline -L $${HOME}/.brew/opt/readline/lib
	@echo "\033[1;34m$(NAME) \033[0;34mhas been compiled"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE)
	@$(CC) $(CFLAGS) -c $< -o $@ -I $${HOME}/.brew/opt/readline/include

lib:
	@[ -d "$(BUILD_DIR)" ] || mkdir "$(BUILD_DIR)"
	@$(MAKE) -C $(LIBFTDIR)

clean:
	@$(MAKE) clean -C $(LIBFTDIR)
	@rm -f $(OBJ)

fclean: clean
	@$(MAKE) fclean -C $(LIBFTDIR)
	@rm -rf $(NAME) $(BUILD_DIR)

re: fclean all

.PHONY: all clean fclean re lib