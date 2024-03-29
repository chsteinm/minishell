/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 20:06:19 by chrstein          #+#    #+#             */
/*   Updated: 2023/11/29 14:10:26 by chrstein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

int	is_backn(t_struct *strct)
{
	if (strct->buf_ptr)
		while (*strct->buf_ptr)
			if (*strct->buf_ptr++ == '\n')
				return (1);
	return (0);
}

char	*ret_line(t_struct *strct)
{
	int	index;

	index = 0;
	if (!strct->tmp)
		return (NULL);
	while (strct->tmp[index] != '\n')
		index++;
	strct->tmp[index + 1] = 0;
	return (strct->tmp);
}

char	*read_and_ret(t_struct *strct, int fd)
{
	strct->buf_ptr = strct->buf;
	strct->bytes_read = read(fd, strct->buf, BUFFER_SIZE);
	if (!strct->bytes_read || !strct->buf[0])
		return (0);
	if (strct->bytes_read == -1)
		return (free(strct->tmp), strct->tmp = 0);
	strct->buf[strct->bytes_read] = 0;
	strct->tmp_tmp = strct->tmp;
	if (strct->tmp)
		strct->tmp = ft_strjoin(strct->tmp, strct->buf);
	else
		strct->tmp = ft_strdup(strct->buf);
	free(strct->tmp_tmp);
	if (!strct->tmp)
		return (0);
	return (strct->tmp);
}

char	*get_next_line(int fd)
{
	static t_struct	strct;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (0);
	if (strct.bytes_read < 1)
	{
		strct.tmp = NULL;
		strct.buf_ptr = NULL;
	}
	else
		strct.tmp = ft_strdup(strct.buf_ptr);
	if (is_backn(&strct))
		return (ret_line(&strct));
	while (read_and_ret(&strct, fd))
		if (is_backn(&strct))
			return (ret_line(&strct));
	if (strct.bytes_read == -1)
		*strct.buf_ptr = 0;
	if (strct.tmp && !strct.tmp[0])
		return (free(strct.tmp), NULL);
	return (strct.tmp);
}
