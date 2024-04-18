/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_replace_white_space.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 02:31:03 by chrstein          #+#    #+#             */
/*   Updated: 2024/04/18 02:31:07 by chrstein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_replace_white_space(char *line)
{
	while (*line)
	{
		if (ft_iswhitespace(*line))
			*line = ' ';
		line++;
	}
}
