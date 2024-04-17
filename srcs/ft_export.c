#include "../includes/minishell.h"

void	ft_export(t_data *data, t_list *node)
{
	int		j_cmd;
	int		j_env;
	char	*tmp;
	size_t	len_var;

	j_cmd = 0;
	while (node->cmd[++j_cmd])
	{
		tmp = ft_strdup(node->cmd[j_cmd]);
		if (!tmp)
			return (perror("Malloc"), close_free_exit(data, FAILURE));
		len_var = 0;
		while (tmp[len_var] != '=')
			if (!tmp[++len_var])
				return ;
		tmp[len_var + 1] = 0;
		j_env = 0;
		while (data->env[j_env] && !ft_strnstr(data->env[j_env], tmp, len_var))
			j_env++;
		free(tmp);
		if (data->env[j_env])
			return ; //
	}
}
