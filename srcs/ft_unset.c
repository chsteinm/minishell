#include "../includes/minishell.h"

void	ft_unset(t_data *data, t_list *node)
{
	int		j_cmd;
	int		j_env;
	char	*var_ptr;
	char	*tmp;
	size_t	len_tmp;

	j_cmd = 0;
	while (node->cmd[++j_cmd])
	{
		tmp = ft_strjoin(node->cmd[j_cmd], "=");
		if (!tmp)
			return (perror("Malloc"), close_free_exit(data, FAILURE));
		len_tmp = ft_strlen(tmp);
		var_ptr = NULL;
		j_env = -1;
		while (!var_ptr && data->env[++j_env])
			var_ptr = ft_strnstr(env[j_env], tmp, len_tmp);
	}
}
