/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_and_null.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 02:32:25 by chrstein          #+#    #+#             */
/*   Updated: 2024/04/18 02:32:28 by chrstein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free_and_null(void *ptr_adrs)
{
	void	**tmp;

	if (ptr_adrs)
	{
		tmp = (void **)ptr_adrs;
		if (*tmp)
		{
			free(*tmp);
			*tmp = NULL;
		}
	}
}
