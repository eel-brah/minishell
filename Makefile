CC := cc
# CFLAGS := -Wall -Wextra -Werror -g
CFLAGS := #-Wall -Wextra -Werror -g -fsanitize=address

PARN_DIR := .
SRC_DIR := $(PARN_DIR)/src
INCLUDE_DIR := $(PARN_DIR)/include
BUILD_DIR := $(PARN_DIR)/build
LIBFTDIR := ./libft

INCLUDE := msh.h
INCLUDE := $(addprefix $(INCLUDE_DIR)/,$(INCLUDE))

SRC := msh.c exec.c exec_utils.c prompt.c setup.c \
	builtins.c expander.c export.c get_next_line.c \
	parsing.c execution.c execution_utils_0.c \
	utils.c utils_0.c utils_1.c utils_2.c \
	execution_utils_1.c free.c errors.c builtins_2.c\
	parsing_utils_0.c parsing_utils_1.c parsing_utils_2.c \
	parsing_utils_3.c ft_setenv.c expander_utils.c expander_utils1.c\
	expander_utils2.c expander_utils3.c expander_utils4.c expander_utils5.c \
	expand_here_doc.c expander_utils6.c expander_utils7.c export_utils.c export_utils1.c \
	setup_utils.c setup_utils1.c
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