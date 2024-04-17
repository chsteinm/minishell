NAME = minishell 
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 #-fsanitize=address 
PATH_SRCS = ./srcs/
SRCS =	main.c \
		close.c \
		parse.c \
		utils.c \
		expand.c \
		error_check.c \
		open_files.c \
		here_doc_manage.c \
		exec.c \
		exec_utils.c \
		ft_exit.c \
		ft_unset.c \
		ft_echo.c
OBJ = $(addprefix $(BUILD_DIR)/,$(SRCS:.c=.o))
BUILD_DIR = .build
LIBFT = ./includes/libft/libft.a

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $@ -lreadline -lhistory
	@echo "\nmimishell is ready for use!\n"

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