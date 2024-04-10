#include "../includes/minishell.h"

char	*replace_var(t_data *data, char **line, char *ptr)
{
	char	*to_rep;
	char	*var;
	size_t	len;

	len = 1;
	while (ptr[len] && !ft_iswhitespace(ptr[len]) && \
	!ft_ismeta(ptr[len]) && ptr[len] != '$' && \
	ptr[len] != '"' && ptr[len] != '\'')
		len++;
	to_rep = ft_strndup(ptr + 1, len);
	if (!to_rep)
		return (NULL);
	to_rep[len - 1] = '=';
	var = NULL;
	data->i = -1;
	while (!var && data->env[++data->i])
		var = ft_strnstr(data->env[data->i], to_rep, len);
	if (var)
		var += len;
	free(to_rep);
	*ptr = 0; //remplace le $ par un \0
	data->tmp = *line;
	*line = join_3_strs(*line, var, ptr + len);
	free(data->tmp);
	return (data->tmp);
}

void	replace_status(t_data *data, char **line, char *ptr)
{
	char	*status;

	status = ft_itoa(data->last_status);
	if (!status)
		return (perror("Malloc"), close_free_exit(data, FAILURE));
	*ptr = 0;
	data->tmp = *line;
	*line = join_3_strs(*line, status, ptr + 2);
	free(data->tmp);
	free(status);
	if (!*line)
		return (perror("Malloc"), close_free_exit(data, FAILURE));
	return (expand(data, line));
}

//replace "$$" by the pid of the curate processus
void	replace_pid(t_data *data, char **line, char *ptr)
{
	char	*pid;
	char	*tmp;

	pid = ft_itoa(data->pid);
	if (!pid)
		return (perror("Malloc"), close_free_exit(data, FAILURE));
	*ptr = 0;
	tmp = *line;
	*line = join_3_strs(*line, pid, ptr + 2);
	free(tmp);
	free(pid);
	if (!*line)
		return (perror("Malloc"), close_free_exit(data, FAILURE));
	return (expand(data, line));
}

void	expand(t_data *data, char **line)
{
	char	*ptr;
	size_t	i;

	ptr = *line;
	i = -1;
	while (ptr[++i])
	{
		if (ptr[i] == '$' && ptr[i + 1] && !is_in_quote(ptr, ptr + i, '\''))
		{
			if (ptr[i + 1] == '$')
				return (replace_pid(data, line, ptr + i));
			else if (ptr[i + 1] == '?')
				return (replace_status(data, line, ptr + i));
			else if (!ft_iswhitespace(ptr[i + 1]) && \
			ptr[i + 1] && ptr[i + 1] != '"')
			{
				if (!replace_var(data, line, ptr + i))
				{
					perror("Malloc");
					return (close_free_exit(data, FAILURE));
				}
				return (expand(data, line));
			}
		}
	}
}
