/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <chrstein@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 02:03:01 by chrstein          #+#    #+#             */
/*   Updated: 2024/06/05 12:54:55 by chrstein         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include <limits.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <signal.h>
# include <sys/types.h>
# include <dirent.h>

# define SIGINT 2
# define SIGOUT 3

# define MUST_EXIT 242

# define IN_D_QUOTE 2
# define IN_S_QUOTE 1

# define STANDBY 0
# define CTRL_C 1
# define IN_LOOP 2

# define ERR_CNF "%s: command not found\n"
# define ERR_DENIED "%s: Permission denied\n"
# define ERR_IS_FILE "%s: Is a directory\n"
# define ERR_SYNTX "syntax error near unexpected token `%c'\n"
# define ERR_SYNTX_NL "syntax error near unexpected token `newline'\n"
# define ERR_QUOTE "quote is not closed\n"
# define ERR_EXIT "exit: too many arguments\n"
# define ERR_EXIT_N "exit: %s: numeric argument required\n"
# define ERR_CD "cd: %s: No such file or directory\n"
# define ERR_CD_TMA "cd: too many arguments\n"
# define ERR_EXP "export: `%s': not a valid identifier\n"

extern int	g_signal;

typedef struct s_data
{
	t_list	*cmd_param;
	t_list	*cmds;
	t_list	*var_no_value;
	char	**env;
	char	**path;
	char	*pwd;
	char	*line;
	char	*no_space_line;
	int		last_status;
	pid_t	pid;
	char	*tmp;
	char	*to_free;
	int		nb_line_hd;
	size_t	nb_cmds;
	size_t	i;
	bool	cancel;
}					t_data;

void	heredoc_handle_signal(t_data *data);
void	signal_set_status(t_data *data);
void	ignore_signal(t_data *data);

void	give_env_path(t_data *data);
void	handle_signal(t_data *data);
bool	check_quote(t_data *data);
int		check_syntax(t_data *data);
int		is_in_quote(char *line, int index);
void	expand(t_data *data, char **line);
char	*ft_join_3_strs(char *s1, char *s2, char *s3);
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
void	final_free(t_data *data);
void	exec_check_file_error(t_data *data, t_list *node);

void	ft_cd(t_data *data, t_list *node);
int		change_dir_cd_path(t_data *data, char *path);
void	update_env_path(t_data *data, char *old);
void	add_var_to_env(t_data *data, char **var_addrs);
void	ft_export_env(t_data *data, char *name, char *value);
int		ft_pwd(t_data *data, t_list *node);
char	*ft_getenv(char **env, char *name);
void	ft_export(t_data *data, t_list *node);
char	*add_to_list(t_data *data, char *var);
void	export_print(t_data *data);
void	ft_echo(t_data *data, t_list *node);
void	ft_exit(t_data *data, t_list *node);
void	ft_unset(t_data *data, t_list *node);

#endif
