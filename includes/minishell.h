#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include "get_next_line_bonus.h"
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

typedef struct s_cmds
{
	char			**cmd;
	char			**envs; 
	char  			*file_in;
	char			*file_out;
	bool			erase_out;
	int				*pipe;
	char			*lim;
	struct s_cmds	*next;
	struct s_cmds	*prev;
}	t_cmds;

typedef struct s_data
{
	t_cmds	*head;
	char 	**env;
	char	*line;
	char	*no_space_line;
	char	**splited_line;
	int		last_signal;
}					t_data;

#endif
