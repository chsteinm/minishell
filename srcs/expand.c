#include "../includes/minishell.h"

void	replace_var(t_data *data, char *ptr)
{
	char	*to_rep;
	char	*var;
	size_t	len;
	size_t	i;

	len = 0;
	while (ptr[++len] && !ft_iswhitespace(ptr[len]) && \
	!ft_ismeta(ptr[len]) && ptr[len] != '$' && ptr[len] != '"')
		continue;
	to_rep = ft_calloc(len + 1, 1);
	if (!to_rep)
		return (perror("Malloc"), close_free_exit(data, EXIT_FAILURE));
	ft_strlcpy(to_rep, ptr + 1, len);
	to_rep[len - 1] = '=';
	var = NULL;
	i = -1;
	while (!var && data->env[++i])
		var = ft_strnstr(data->env[i], to_rep, len);
	if (var)
		var += len;
	free(to_rep);
	*ptr = 0;
	ptr[len - 1] = 0;
	data->tmp = data->line;
	data->line = join_3_strs(data->line, var, ptr + len);
	free(data->tmp);
	if (!data->line)
		return (perror("Malloc"), close_free_exit(data, EXIT_FAILURE));
}

void	replace_pid(t_data *data, char *ptr)
{
	char	*pid;
	char	*tmp;

	pid = ft_itoa(data->pid);
	if (!pid)
		return (perror("Malloc"), close_free_exit(data, EXIT_FAILURE));
	*ptr = 0;
	ptr[1] = 0;
	tmp = data->line;
	data->line = join_3_strs(data->line, pid, ptr + 2);
	free(tmp);
	free(pid);
	if (!data->line)
		return (perror("Malloc"), close_free_exit(data, EXIT_FAILURE));
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
			{
				replace_pid(data, ptr + i);
				return (expand(data));
			}
			else if (ft_iswhitespace(ptr[i + 1]) || ft_ismeta(ptr[i + 1]))
				continue;
			else if (ptr[i + 1] != '"' && ptr[i + 1] != '\'')
			{
				replace_var(data, ptr + i);
				return (expand(data));
			}
		}
	}
}
