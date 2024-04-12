#include "../includes/minishell.h"

static char	*ft_get_env_name(t_data *data, char **str);
static int	ft_parse_env(char *name, char *value);
static void	undo_split(t_list *node);
void		ft_add_env(t_data *data, char *to_add);
void		free_tab(char **tab);
void		malloc_error(t_data *data);
int			ft_isempty(char *str);


void	ft_export(t_data *data, t_list *node)
{
	char	*name;

	undo_split(node);
	name = ft_get_env_name(data, &node->cmd[1]);
	if (!name)
		return ;
	if (ft_parse_env(name, node->cmd[1]) == 1)
	{
		free(name);
		data->last_status = 1;
		return ;
	}
	if (ft_getenv(data->env, name))
	{
		ft_export_env(data, name, node->cmd[1]);
		free(name);
		return ;
	}
	else
	{
		ft_add_env(data, node->cmd[1]);
		free(name);
	}
}

static void	undo_split(t_list *node)
{
	int		i;
	char	*tmp;

	i = 1;
	tmp = ft_strdup(node->cmd[i]);
	while (node->cmd[i])
	{
		if (node->cmd[i + 1])
		{
			tmp = ft_strjoin(tmp, node->cmd[i + 1]);
			i++;
			continue ;
		}
		i++;
	}
	free(node->cmd[1]);
	node->cmd[1] = tmp;
}

static int	ft_parse_env(char *name, char *value)
{
	if (ft_isempty(name))
	{
		ft_putstr_fd("export: not a valid identifier\n", 2);
		return (1);
	}
	while (*name)
	{
		if (ft_isalpha(*name) || *name == '_' || *name == '=')
			name++;
		else
		{
			ft_putstr_fd("export: not a valid identifier\n", 2);
			return (1);
		}
	}
	while (*value++)
	{
		if (!ft_isascii(*value++))
		{
			ft_putstr_fd("export: not a valid identifier\n", 2);
			return (1);
		}
	}
	return (0);
}

int	ft_isempty(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '=')
			return (0);
		i++;
	}
	return (1);
}

static char	*ft_get_env_name(t_data *data, char **str)
{
	char	*name;
	char	*value;
	int		i;

	i = 0;
	if (!*str)
		return (NULL);
	if (ft_strchr(*str, '='))
	{
		while ((*str)[i] != '=')
			i++;
		name = ft_substr(*str, 0, i + 1);
		if (!name)
			malloc_error(data);
		value = ft_strdup(*str + i + 1);
		if (!value)
			malloc_error(data);
		free(*str);
		*str = value;
	}
	else
		name = ft_strdup(*str);
	return (name);
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
			// free(data->env[i]);
			// LEEK ?
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
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

void	ft_add_env(t_data *data, char *to_add)
{
	char	**new_env;
	int		i;

	i = 0;
	while (data->env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		malloc_error(data);
	i = -1;
	while (data->env[++i])
	{
		new_env[i] = ft_strdup(data->env[i]);
		if (!new_env[i])
			malloc_error(data);
	}
	new_env[i] = ft_strdup(to_add);
	if (!new_env[i])
		malloc_error(data);
	// free_tab(data->env); // TODO: fix leak
	new_env[i + 1] = NULL;
	data->env = new_env;
}

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	if (tab)
	{
		while (tab[i])
		{
			if (tab[i])
				free(tab[i]);
			i++;
		}
		free(tab);
	}
}

void	malloc_error(t_data *data)
{
	ft_putstr_fd("malloc failed\n", 2);
	close_free_exit(data, FAILURE);
}
