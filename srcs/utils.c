#include "../includes/minishell.h"

int	is_quote_open(int actual_quote, int other_quote, int i_quote, int i_char)
{
	if (actual_quote == 0 && other_quote == 0 && i_quote != i_char)
		actual_quote = 1;
	else
		actual_quote = 0;
	return (actual_quote);
}

int	is_in_quote(char *line, int index)
{
	int	i;
	int	s_quotes;
	int	d_quotes;

	i = -1;
	s_quotes = 0;
	d_quotes = 0;
	while (line[++i] && i <= index)
	{
		if (line[i] == '\'')
			s_quotes = is_quote_open(s_quotes, d_quotes, i, index);
		if (line[i] == '"')
			d_quotes = is_quote_open(d_quotes, s_quotes, i, index);
	}
	if (d_quotes != 0)
		return (IN_D_QUOTE);
	if (s_quotes != 0)
		return (IN_S_QUOTE);
	return (0);
}

int	is_sep(char *ptr, int i)
{
	if (!is_in_quote(ptr, i))
		if (ft_iswhitespace(ptr[i]) || ft_ismeta(ptr[i]))
			return (1);
	return (0);
}

void	strdelquotes(char *str, char *ptr, size_t len)
{
	size_t	i;
	size_t	i_str;

	i_str = 0;
	i = -1;
	while (++i < len)
		if ((ptr[i] != '"' && ptr[i] != '\'') || is_in_quote(ptr, i))
			str[i_str++] = ptr[i];
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
