/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_one_str_in_strs.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 02:32:13 by chrstein          #+#    #+#             */
/*   Updated: 2024/04/18 02:32:18 by chrstein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free_one_str_in_strs(char **strs, int i_str)
{
	char	*cpy;

	ft_free_and_null(strs + i_str);
	while (strs[i_str++ + 1])
	{
		cpy = strs[i_str];
		strs[i_str] = strs[i_str - 1];
		strs[i_str - 1] = cpy;
	}
}
