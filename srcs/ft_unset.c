#include "../includes/minishell.h"

void	ft_unset(t_data *data, t_list *node)
{
	int		j_cmd;
	int		j_env;
	char	*tmp;
	size_t	len_tmp;

	j_cmd = 0;
	while (node->cmd[++j_cmd])
	{
		tmp = ft_strjoin(node->cmd[j_cmd], "=");
		if (!tmp)
			return (perror("Malloc"), close_free_exit(data, FAILURE));
		len_tmp = ft_strlen(tmp);
		j_env = 0;
		while (data->env[j_env] && !ft_strnstr(data->env[j_env], tmp, len_tmp))
			j_env++;
		free(tmp);
		if (data->env[j_env])
			ft_free_one_str_in_strs(data->env, j_env);
	}
}
