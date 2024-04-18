/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_skip_wspaces.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 02:31:17 by chrstein          #+#    #+#             */
/*   Updated: 2024/04/18 02:31:19 by chrstein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_skip_wspaces(char **ptr)
{
	while (ft_iswhitespace(**ptr))
		(*ptr)++;
}
