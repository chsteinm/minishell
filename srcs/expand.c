#include "../includes/minishell.h"

char	*replace_var(t_data *data, char *ptr)
{
	char	*to_rep;
	char	*var;
	size_t	len;
	size_t	i;

	len = 1;
	while (ptr[len] && !ft_iswhitespace(ptr[len]) && \
	!ft_ismeta(ptr[len]) && ptr[len] != '$' && ptr[len] != '"')
		len++;
	to_rep = ft_strndup(ptr + 1, len);
	if (!to_rep)
		return (NULL);
	to_rep[len - 1] = '=';
	var = NULL;
	i = -1;
	while (!var && data->env[++i])
		var = ft_strnstr(data->env[i], to_rep, len);
	if (var)
		var += len;
	free(to_rep);
	*ptr = 0; //remplace le $ par un \0
	data->tmp = data->line;
	data->line = join_3_strs(data->line, var, ptr + len);
	free(data->tmp);
	return (data->tmp);
}

void	replace_status(t_data *data, char *ptr)
{
	char	*status;

	status = ft_itoa(data->last_status);
	if (!status)
		return (perror("Malloc"), close_free_exit(data, EXIT_FAILURE));
	*ptr = 0;
	data->tmp = data->line;
	data->line = join_3_strs(data->line, status, ptr + 2);
	free(data->tmp);
	free(status);
	if (!data->line)
		return (perror("Malloc"), close_free_exit(data, EXIT_FAILURE));
	return (expand(data));
}

//replace "$$" by the pid of the curate processus
void	replace_pid(t_data *data, char *ptr)
{
	char	*pid;
	char	*tmp;

	pid = ft_itoa(data->pid);
	if (!pid)
		return (perror("Malloc"), close_free_exit(data, EXIT_FAILURE));
	*ptr = 0;
	tmp = data->line;
	data->line = join_3_strs(data->line, pid, ptr + 2);
	free(tmp);
	free(pid);
	if (!data->line)
		return (perror("Malloc"), close_free_exit(data, EXIT_FAILURE));
	return (expand(data));
}

void	expand(t_data *data)
{
	char	*ptr;
	size_t	i;

	ptr = data->line;
	i = -1;
	while (ptr[++i])
	{
		if (ptr[i] == '$' && ptr[i + 1] && !is_in_quote(ptr, ptr + i, '\''))
		{
			if (ptr[i + 1] == '$')
				return (replace_pid(data, ptr + i));
			else if (ptr[i + 1] == '?')
				return (replace_status(data, ptr + i));
			else if (ptr[i + 1] != '"' && ptr[i + 1] != '\'')
			{
				if (!replace_var(data, ptr + i))
					return (perror("Malloc"), close_free_exit(data, EXIT_FAILURE));
				return (expand(data));
			}
		}
	}
}
