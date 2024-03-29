NAME = minishell
NAME_BONUS = 
CC = cc
CFLAGS = -Wall -Wextra -Werror -lreadline -g3 -fsanitize=address 
PATH_SRCS = ./srcs/
SRCS = main.c
# SRCS_BONUS = main_bonus.c exec_bonus.c set_bonus.c get_next_line_bonus.c here_doc_manage_bonus.c
OBJ = $(addprefix $(BUILD_DIR)/,$(SRCS:.c=.o))
# OBJ_BONUS = $(addprefix $(BUILD_DIR)/,$(SRCS_BONUS:.c=.o))
BUILD_DIR = .build
LIBFT = ./includes/libft/libft.a

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $@
	@echo "\nmimishell is ready for use!\n"
# @echo 'tape "./pipex file1 cmd1 cmd2 file2"'"\n"

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(OBJ_BONUS)
	@$(CC) $(CFLAGS) $(OBJ_BONUS) $(LIBFT) -o $@
	@echo "\npipex is ready for use!\n"
	@echo 'tape "./pipex_bonus file1 cmd1 ... cmdn file2"'"\n"

$(BUILD_DIR)/%.o: $(PATH_SRCS)%.c Makefile $(LIBFT)
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@ -I ./includes

-include $(OBJ:.o=.d) $(OBJ_BONUS:.o=.d)

$(LIBFT): FORCE
	@make --no-print-directory -C includes/libft bonus

clean:
	@make --no-print-directory -C ./includes/libft clean
	@rm -rf $(BUILD_DIR)

fclean: clean
	@make --no-print-directory -C ./includes/libft fclean
	@rm -rf $(NAME) $(NAME_BONUS)

re: fclean
	@make --no-print-directory

FORCE:

.PHONY: all clean fclean re bonus FORCE