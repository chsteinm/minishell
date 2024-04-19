/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_print.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guilrodr <guilrodr@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 21:13:54 by chrstein          #+#    #+#             */
/*   Updated: 2024/04/18 21:37:11 by guilrodr         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_var_like_export(char *str)
{
	ft_printf("declare -x ");
	while (*str && *str != '=')
		write(1, str++, 1);
	write(1, str, 1);
	if (*str++)
	{
		write(1, "\"", 1);
		ft_printf("%s", str);
		write(1, "\"", 1);
	}
	write(1, "\n", 1);
}

void	export_print(t_data *data)
{
	char	**tmp_env;
	size_t	len;
	t_list	*var_node;

	len = ft_strssize(data->env) + ft_lstsize(data->var_no_value);
	tmp_env = ft_strsdup(data->env, len);
	if (!tmp_env)
		return (perror("Malloc"), close_free_exit(data, MUST_EXIT));
	var_node = data->var_no_value;
	while (var_node)
	{
		tmp_env[ft_strssize(tmp_env)] = ft_strdup(var_node->content);
		var_node = var_node->next;
	}
	ft_sort_params(len, tmp_env);
	len = -1;
	while (tmp_env[++len])
		print_var_like_export(tmp_env[len]);
	ft_free_strings(tmp_env);
}

char	*add_to_list(t_data *data, char *var)
{
	char	*var_dup;

	var_dup = ft_strdup(var);
	if (!var_dup)
		return (perror("Malloc"), close_free_exit(data, MUST_EXIT), NULL);
	ft_lstadd_back(&data->var_no_value, ft_lstnew(var_dup));
	return (var_dup);
}
