#include "../includes/minishell.h"

void	error(t_data *data, int error, char c)
{
	if (error == 2 && c)
		ft_dprintf(2, ERR_SYNTX, c);
	else if (error == 2 && !c)
		ft_dprintf(2, ERR_SYNTX_NL);
	else if (error == 'q')
		ft_dprintf(2, ERR_QUOTE);
	if (error != 2)
		data->last_signal = 1;
	else
		data->last_signal = error;
}

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
		while ((*ptr)[len] && \
		!ft_iswhitespace((*ptr)[len]) && !ft_ismeta((*ptr)[len]))
			len++;
	str = ft_strndup(*ptr, len);
	if (!str)
		return (perror("Malloc"), close_free_exit(data, EXIT_FAILURE), NULL);
	if (quote)
		*ptr = (*ptr) + 1;
	*ptr = (*ptr) + len;
	return (str);
}
