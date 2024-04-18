/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <chrstein@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 15:30:15 by chrstein          #+#    #+#             */
/*   Updated: 2024/04/18 21:34:26 by chrstein         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	update_var(t_data *data, t_list *node, int j_cmd, int j_env)
{
	free(data->env[j_env]);
	data->env[j_env] = node->cmd[j_cmd];
	node->cmd[j_cmd] = NULL;
}

bool	is_valid_identifier(t_data *data, char *var)
{
	int	i;

	i = 0;
	if (!ft_isalpha(var[i]))
	{
		ft_dprintf(2, ERR_EXP, var);
		data->last_status = 1;
		return (0);
	}
	while (var[++i])
	{
		if (var[i] == '=')
			return (1);
		if (!ft_isalnum(var[i]))
		{
			ft_dprintf(2, ERR_EXP, var);
			data->last_status = 1;
			return (0);
		}
	}
	return (1);
}

char	*init_var_name(t_data *data, char *to_export, int *len)
{
	char	*var_name;
	int		len_var;

	var_name = ft_strdup(to_export);
	if (!var_name)
		return (perror("Malloc"), close_free_exit(data, MUST_EXIT), NULL);
	len_var = 0;
	while (var_name[len_var] && var_name[len_var] != '=')
		len_var++;
	if (!(var_name[len_var]))
		return (free(var_name), var_name = NULL, NULL);
	var_name[++len_var] = 0;
	*len = len_var;
	return (var_name);
}

void	add_var_to_env(t_data *data, t_list *node, int j_cmd)
{
	char	**new_env;
	size_t	len_env;

	len_env = ft_strssize(data->env);
	new_env = ft_strsdup(data->env, len_env + 1);
	if (!new_env)
		return (perror("Malloc"), close_free_exit(data, MUST_EXIT));
	new_env[len_env] = node->cmd[j_cmd];
	node->cmd[j_cmd] = NULL;
	ft_free_strings(data->env);
	data->env = new_env;
}

void	ft_export(t_data *data, t_list *node)
{
	int		j_cmd;
	int		j_env;
	char	*var_name;
	int		len_var;

	j_cmd = 0;
	if (node->cmd[1] == NULL)
		return (export_print(data));
	while (node->cmd[++j_cmd])
	{
		if (!is_valid_identifier(data, node->cmd[j_cmd]))
			continue ;
		var_name = init_var_name(data, node->cmd[j_cmd], &len_var);
		if (!var_name && add_to_list(data, node->cmd[j_cmd]))
			continue ;
		j_env = 0;
		while (data->env[j_env] && \
		!ft_strnstr(data->env[j_env], var_name, len_var))
			j_env++;
		free(var_name);
		if (data->env[j_env])
			update_var(data, node, j_cmd, j_env);
		else
			add_var_to_env(data, node, j_cmd);
	}
}
