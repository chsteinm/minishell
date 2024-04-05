#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include "get_next_line.h"
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>

# define TRUE 1
# define FALSE 0
# define CLEAR " \e[1;H\e[2J"
# define ERR_CNF "shell: %s: command not found\n"
# define ERR_SYNTX "shell: syntax error near unexpected token '%c'\n"
# define ERR_SYNTX_NL "shell: syntax error near unexpected token 'newline'\n"
# define ERR_QUOTE "shell: quote is not closed\n"
# define ERR_M_OR_L "Malloc failed or no limiter found on here_doc\n"

typedef struct s_data
{
	t_list	*cmd_param;
	t_list	*cmds;
	char 	**env;
	char	*line;
	char	*no_space_line;
	char	*no_w_space_line;
	char	**splited_line;
	int		last_status;
	pid_t	pid;
	char	*tmp;
}					t_data;

bool	check_quote(t_data *data);
int		check_syntax(t_data *data);
void	expand(t_data *data);
void	parse(t_data *data);
char	*str__dup(t_data *data, char **ptr);
char	*join_3_strs(char *s1, char *s2, char *s3);
int		is_in_quote(char *line, char *ptr, char q);
void	error(t_data *data, int error, char c);
void	close_free_exit(t_data *data, int ret);

#endif
