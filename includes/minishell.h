#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
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
# include <signal.h>

# define SUCCESS 0
# define FAILURE 1

# define SIGINT 2
# define SIGOUT 3

# define ERR_CNF "%s: command not found\n"
# define ERR_DENIED "%s: Permission denied\n"
# define ERR_SYNTX "syntax error near unexpected token `%c'\n"
# define ERR_SYNTX_NL "syntax error near unexpected token `newline'\n"
# define ERR_QUOTE "quote is not closed\n"
# define ERR_HERE_DOC "warning: here-document at line %d delimited by end-of-file (wanted `%s')\n"

typedef struct s_data
{
	t_list	*cmd_param;
	t_list	*cmds;
	char	**env;
	char	**path;
	char	*line;
	char	*pwd;
	char	*no_space_line;
	char	*no_w_space_line;
	char	**splited_line;
	int		last_status;
	pid_t	pid;
	char	*tmp;
	int		nb_line_hd;
	size_t	nb_cmds;
	size_t	i;
}					t_data;

bool	check_quote(t_data *data);
int		check_syntax(t_data *data);
int		is_in_quote(char *line, char *ptr, char q);
void	expand(t_data *data, char **line);
char	*join_3_strs(char *s1, char *s2, char *s3);
void	parse(t_data *data);
char	*str__dup(t_data *data, char **ptr);
void	open_out(t_data *data, t_list *node, char **ptr);
void	open_in(t_data *data, t_list *node, char **ptr);
void	here_doc_manage(t_data *data, t_list *node);
void	exec(t_data *data, t_list *cmd);
void	make_pipes(t_data *data, t_list *node);
bool	to_exec(t_list *node);
void	find_good_path(t_data *data, t_list *node);
void	make_dup2(t_data *data, t_list *node);
void	error(t_data *data, int error, char c);
void	close_free_exit(t_data *data, int ret);
void	close_all_fds(t_list *head);
void	close_fds(t_list *node);

int		ft_cd(t_data *data, t_list *node);
int		ft_pwd(t_data *data, t_list *node);
void	ft_export_env(t_data *data, char *name, char *value);
char	*ft_getenv(char **env, char *name);
void	ft_export(t_data *data, t_list *node);



#endif
