#include "../includes/minishell.h"

int		is_in_quote(char *line, char *ptr, char q)
{
	int s_quote_left;
	int s_quote_right;

	s_quote_left = 0;
	s_quote_right = 0;
	while (line != ptr)
		if (*line++ == q)
			s_quote_left++;
	while (*ptr)
		if (*ptr++ == q)
			s_quote_right++;
	return (s_quote_left % 2 && s_quote_right % 2);
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

void	strdelquotes(char *str)
{
	char	*ptr;

	ptr = str;
	while (*str)
	{
		if (*str != '"' && *str != '\'')
			*ptr++ = *str;
		str++;
	}
	*ptr = 0;
}

char	*str__dup(t_data *data, char **ptr)
{
	char	quote;
	size_t	len;
	char	*str;

	len = 0;
	quote = 0;
	if (**ptr == '"' || **ptr == '\'')
	{
		quote = **ptr;
		*ptr = (*ptr) + 1;
		while ((*ptr)[len] != quote)
			len++;
	}
	else
		while ((*ptr)[len] && !is_sep(*ptr, len))
			len++;
	str = ft_strndup(*ptr, len);
	if (!str)
		return (perror("Malloc"), close_free_exit(data, EXIT_FAILURE), NULL);
	if (quote)
		*ptr = (*ptr) + 1;
	*ptr = (*ptr) + len;
	strdelquotes(str);
	return (str);
}
