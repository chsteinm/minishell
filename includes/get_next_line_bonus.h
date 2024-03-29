/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrstein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 20:06:53 by chrstein          #+#    #+#             */
/*   Updated: 2023/11/29 14:13:35 by chrstein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

# include <unistd.h>
# include <stdlib.h>

typedef struct s_struct
{
	char	buf[BUFFER_SIZE + 1];
	char	*tmp;
	char	*tmp_tmp;
	char	*buf_ptr;
	ssize_t	bytes_read;
}	t_struct;

char	*get_next_line(int fd);

#endif
