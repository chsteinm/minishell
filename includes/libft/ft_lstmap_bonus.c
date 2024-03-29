/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:03:39 by chrstein          #+#    #+#             */
/*   Updated: 2023/11/06 16:04:18 by chrstein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*begin_new;
	t_list	*new;
	void	*new_content;

	begin_new = NULL;
	while (lst)
	{
		new_content = (*f)(lst->content);
		new = ft_lstnew(new_content);
		if (!new)
		{
			free(new_content);
			ft_lstclear(&begin_new, del);
			return (NULL);
		}
		ft_lstadd_back(&begin_new, new);
		lst = lst->next;
	}
	return (begin_new);
}
