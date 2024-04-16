#include "../includes/minishell.h"

bool	is_in_quote(char *line, char *ptr, char q)
{
	bool	in_quote;
	bool	in_s_quote;

	in_quote = FALSE;
	in_s_quote = FALSE;
	while (line != ptr)
	{
		if (*line == '"' && !in_s_quote)
			in_quote = (in_quote == FALSE);
		if (*line == '\'' && !in_quote)
			in_s_quote = (in_s_quote == FALSE);
		line++;
	}
	if (q == '"' && in_quote)
		return (TRUE);
	if (q == '\'' && in_s_quote)
		return (TRUE);
	return (FALSE);
}

char	*join_3_strs(char *s1, char *s2, char *s3)
{
	char	*ret;
	char	*tmp;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	ret = ft_strjoin(tmp, s3);
	free(tmp);
	return (ret);
}

int		is_sep(char *ptr, int i)
{
	if (!is_in_quote(ptr, ptr + i, '"') && \
		!is_in_quote(ptr, ptr + i, '\''))
		if (ft_iswhitespace(ptr[i]) || ft_ismeta(ptr[i]))
			return (1);
	return (0);
}

//supprime les quotes qui ne sont pas dans des quotes
void	strdelquotes(char *str, char *ptr, size_t len)
{
	size_t	i;
	size_t	i_str;

	i_str = 0;
	i = -1;
	while (++i < len)
	{
		if ((ptr[i] != '"' && ptr[i] != '\'') || \
		is_in_quote(ptr, ptr + i, '"') || is_in_quote(ptr, ptr + i, '\''))
				str[i_str++] = ptr[i];
	}
	str[i_str] = 0;
}

char	*str__dup(t_data *data, char **ptr)
{
	size_t	len;
	char	*str;

	len = 0;
	while ((*ptr)[len] && !is_sep(*ptr, len))
		len++;
	str = ft_strndup(*ptr, len);
	if (!str)
		return (perror("malloc"), close_free_exit(data, EXIT_FAILURE), NULL);
	strdelquotes(str, *ptr, len);
	*ptr = (*ptr) + len;
	return (str);
}
