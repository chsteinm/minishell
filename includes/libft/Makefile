NAME = libft.a
CFLAGS = -Wall -Wextra -Werror
SRCS = 	ft_atol.c \
		ft_bzero.c \
		ft_calloc.c \
		ft_isalnum.c \
		ft_isalpha.c \
		ft_isascii.c \
		ft_isdigit.c \
		ft_isprint.c \
		ft_ismeta.c \
		ft_iswhitespace.c \
		ft_replace_white_space.c \
		ft_skip_wspaces.c \
		ft_itoa.c \
		ft_memchr.c \
		ft_memcmp.c \
		ft_memcpy.c \
		ft_memmove.c \
		ft_memset.c \
		ft_putchar_fd.c \
		ft_putendl_fd.c \
		ft_putnbr_fd.c \
		ft_putstr_fd.c \
		ft_split.c \
		ft_strchr.c \
		ft_strdup.c \
		ft_strdelspace.c \
		ft_striteri.c \
		ft_strjoin.c \
		ft_strlcat.c \
		ft_strlcpy.c \
		ft_strlen.c \
		ft_strmapi.c \
		ft_strncmp.c \
		ft_strnstr.c \
		ft_strndup.c \
		ft_rstrnstr.c \
		ft_strrchr.c \
		ft_strtrim.c \
		ft_substr.c \
		ft_tolower.c \
		ft_toupper.c \
		ft_itoa_base.c \
		ft_free_strings.c \
		ft_free_and_null.c \
		ft_strsdup.c \
		ft_printf.c \
		ft_dprintf.c \
		ft_print_conversions.c \
		ft_dprint_conversions.c \
		ft_utoa.c \
		ft_putnbr_base.c \
		ft_putnbr_base_fd.c \
		ft_printstrs.c \
		get_next_line.c
OBJ = $(addprefix $(BUILD_DIR)/,$(SRCS:.c=.o))
SRCS_BONUS =	ft_lstadd_back_bonus.c \
				ft_lstadd_front_bonus.c \
				ft_lstclear_bonus.c \
				ft_lstdelone_bonus.c \
				ft_lstiter_bonus.c \
				ft_lstlast_bonus.c \
				ft_lstmap_bonus.c \
				ft_lstnew_bonus.c \
				ft_lstsize_bonus.c
OBJ_BONUS = $(addprefix $(BUILD_DIR)/,$(SRCS_BONUS:.c=.o))
BUILD_DIR = .build

all: $(NAME)

$(NAME): $(OBJ) Makefile
	@ar rcs $@ $^

bonus: $(NAME) $(OBJ_BONUS)
	@if [ "$$(find $(OBJ_BONUS) -newer "$(NAME)" -print -quit)" ]; then \
		ar rcs $(NAME) $^; fi
#	  else \
	  	echo "libft make: Nothing to be done for 'bonus'."; \
	 fi

$(BUILD_DIR)/%.o: %.c Makefile
	@mkdir -p $(BUILD_DIR)
	@cc $(CFLAGS) -MMD -MP -c $< -o $@

-include $(OBJ:.o=.d) $(OBJ_BONUS:.o=.d)

clean:
	@rm -rf $(BUILD_DIR)

fclean: clean
	@rm -rf $(NAME)

re : fclean
	@make

.PHONY: all clean fclean re bonus
