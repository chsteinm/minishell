/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_one_str_in_strs.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <chrstein@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 02:32:13 by chrstein          #+#    #+#             */
/*   Updated: 2024/04/23 00:37:57 by chrstein         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free_one_str_in_strs(char **strs, int i_str)
{
	char	*cpy;

	if (!strs[i_str])
		return ;
	ft_free_and_null(strs + i_str);
	while (strs[i_str++ + 1])
	{
		cpy = strs[i_str];
		strs[i_str] = strs[i_str - 1];
		strs[i_str - 1] = cpy;
	}
}
