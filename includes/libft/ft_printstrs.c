/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printstrs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 03:45:40 by chrstein          #+#    #+#             */
/*   Updated: 2024/02/16 03:46:19 by chrstein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_printstrs(char **strs)
{
	size_t	i;

	i = -1;
	if (!strs)
		ft_printf("(null)");
	while (strs && strs[++i])
		ft_printf("%s\n", strs[i]);
}
