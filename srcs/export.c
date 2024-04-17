#include "../includes/minishell.h"

void	malloc_error(t_data *data)
{
	ft_putstr_fd("malloc failed\n", 2);
	close_free_exit(data, FAILURE);
}

void	ft_export_env(t_data *data, char *name, char *value)
{
	int		i;
	char	*tmp;

	i = 0;
	while (data->env[i])
	{
		if (!(ft_strncmp(data->env[i], name, ft_strlen(name))))
		{
			tmp = ft_strdup(name);
			if (!tmp)
				malloc_error(data);
			ft_free_and_null((void **)&data->env[i]);
			if (ft_strncmp(tmp, value, ft_strlen(value)))
				data->env[i] = ft_strjoin(tmp, value);
			else
				data->env[i] = ft_strdup(tmp);
			if (!data->env[i])
				malloc_error(data);
			free(tmp);
		}
		i++;
	}
}

char	*ft_getenv(char **env, char *name)
{
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(name);
	while (env[i])
	{
		if (!ft_strncmp(env[i], name, len))
			return (env[i] + len);
		i++;
	}
	return (NULL);
}
