/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 18:05:40 by chrstein          #+#    #+#             */
/*   Updated: 2024/02/06 17:23:44 by chrstein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

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

typedef struct s_data
{
	int		fd_in;
	int		fd_out;
	int		fildes[2];
	pid_t	pid;
	pid_t	pid2;
	int		status;
	int		status_last;
	char	**args_in;
	char	**args_out;
	char	**argv_ptr;
	char	**path;
	int		is_env;
	int		is_path;
}					t_data;

int	close_n_exit(t_data *data, int ret);
int	exec_first(t_data *data, char **env);
int	exec_second(t_data *data, char **env);

#endif
