#include "../includes/minishell.h"

int	check_space(t_data *data)
{
	size_t	j;

	data->no_w_space_line = ft_strdup(data->line);
	if (!data->no_w_space_line)
		return (perror("Malloc"), close_free_exit(data, EXIT_FAILURE), 1);
	ft_replace_white_space(data->no_w_space_line);
	data->splited_line = ft_split(data->no_w_space_line, ' ');
	j = 0;
	while (data->splited_line[++j])
		if (ft_ismeta(data->splited_line[j][ft_strlen(data->splited_line[j]) - 1]))
			if (ft_ismeta(data->splited_line[j-1][ft_strlen(data->splited_line[j - 1]) - 1]))
				if (data->splited_line[j-1][ft_strlen(data->splited_line[j]) - 1] != '|')
					return (error(data, 2, data->splited_line[j][0]), 1);
	j = 0;
	while (data->splited_line[++j])
		if (ft_ismeta(data->splited_line[j][0]))
			if (ft_ismeta(data->splited_line[j-1][ft_strlen(data->splited_line[j-1]) - 1]))
				if (data->splited_line[j-1][ft_strlen(data->splited_line[j-1]) - 1] != '|')
					return (error(data, 2, data->splited_line[j][0]), 1);
	return (0);
}

int	check_syntax(t_data *data)
{
	char *ptr;

	
	data->no_space_line = ft_strdelspace(data->line);
	if (!data->no_space_line)
		return (perror("Malloc"), close_free_exit(data, EXIT_FAILURE), 1);
	ptr = data->no_space_line;
	while (*ptr)
	{
		if (ft_ismeta(*ptr) && \
		!is_in_quote(data->no_space_line, ptr, '\'') && \
		!is_in_quote(data->no_space_line, ptr, '"'))
		{
			if (*ptr == '|' && (ptr[1] == '|' || \
			ptr[1] == '\0' || ptr == data->no_space_line))
				return (error(data, 2, *ptr), 1);
			else if (*ptr != ptr[1] && *ptr != '|' && \
			(ft_ismeta(ptr[1]) || ptr[1] == '\0'))
				return (error(data, 2, ptr[1]), 1);
			else if (*ptr == ptr[1] && (ft_ismeta(ptr[2]) || ptr[1] == '\0'))
				return (error(data, 2, ptr[2]), 1);
		}
		ptr++;
	}
	// return (0);
	return (check_space(data));
}

bool	check_quote(t_data *data)
{
	bool	in_quote;
	bool	in_s_quote;
	char	*ptr;

	in_quote = FALSE;
	in_s_quote = FALSE;
	ptr = data->line;
	while (*ptr)
	{
		if (*ptr == '"' && !in_s_quote)
		{
			if (!in_quote)
				in_quote = TRUE;
			else
				in_quote = FALSE;
		}
		if (*ptr++ == '\'' && !in_quote)
		{
			if (!in_s_quote)
				in_s_quote = TRUE;
			else
				in_s_quote = FALSE;
		}
	}
	return (in_s_quote || in_quote);
}