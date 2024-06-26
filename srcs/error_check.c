/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <chrstein@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 15:29:17 by chrstein          #+#    #+#             */
/*   Updated: 2024/04/18 15:33:51 by chrstein         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	error(t_data *data, int error, char c)
{
	if (error == 2 && c)
		ft_dprintf(2, ERR_SYNTX, c);
	else if (error == 2 && !c)
		ft_dprintf(2, ERR_SYNTX_NL);
	else if (c == 'q')
		ft_dprintf(2, ERR_QUOTE);
	data->last_status = error;
}

int	check_space(t_data *data)
{
	char	*tmp;
	int		index;

	tmp = data->line;
	index = 0;
	while (*tmp)
	{
		if ((*tmp == '>' || *tmp == '<') && ft_iswhitespace(*(tmp + 1)) && \
		!is_in_quote(data->line, ft_strchr_index(data->line, tmp)))
		{
			while (ft_iswhitespace(tmp[index]))
				index++;
			if (tmp[index] == *tmp && &tmp[index] != tmp)
				return (error(data, 2, *tmp), 1);
			index = 0;
		}
		tmp++;
	}
	return (SUCCESS);
}

int	check_syntax(t_data *data)
{
	char	*ptr;

	data->no_space_line = ft_strdelspace(data->line);
	if (!data->no_space_line)
		return (perror("malloc"), close_free_exit(data, FAILURE), 1);
	ptr = data->no_space_line;
	while (*ptr)
	{
		if (ft_ismeta(*ptr) && \
		!is_in_quote(data->no_space_line, \
		ft_strchr_index(data->no_space_line, ptr)))
		{
			if (*ptr == '|' && (ptr[1] == '|' || \
			ptr[1] == '\0' || ptr == data->no_space_line))
				return (error(data, 2, *ptr), 1);
			else if (*ptr != ptr[1] && *ptr != '|' && \
			(ft_ismeta(ptr[1]) || ptr[1] == '\0'))
				return (error(data, 2, ptr[1]), 1);
			else if (*ptr == ptr[1] && (ft_ismeta(ptr[2]) || ptr[1] == '\0'))
				return (error(data, 2, ptr[2]), 1);
		}
		ptr++;
	}
	return (check_space(data));
}

bool	check_quote(t_data *data)
{
	bool	in_quote;
	bool	in_s_quote;
	char	*ptr;

	in_quote = FALSE;
	in_s_quote = FALSE;
	ptr = data->line;
	while (*ptr)
	{
		if (*ptr == '"' && !in_s_quote)
		{
			if (!in_quote)
				in_quote = TRUE;
			else
				in_quote = FALSE;
		}
		if (*ptr++ == '\'' && !in_quote)
		{
			if (!in_s_quote)
				in_s_quote = TRUE;
			else
				in_s_quote = FALSE;
		}
	}
	return (in_s_quote || in_quote);
}
