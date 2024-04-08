#include "../includes/minishell.h"

void	error(t_data *data, int error, char c)
{
	if (error == 2 && c)
		ft_dprintf(2, ERR_SYNTX, c);
	else if (error == 2 && !c)
		ft_dprintf(2, ERR_SYNTX_NL);
	else if (c == 'q')
		ft_dprintf(2, ERR_QUOTE);
	data->last_status = error;
}
// error if < <... / > >... / 
int	check_space(t_data *data)
{
	size_t	j;

	data->no_w_space_line = ft_strdup(data->line);
	if (!data->no_w_space_line)
		return (perror("Malloc"), close_free_exit(data, EXIT_FAILURE), 1);
	ft_replace_white_space(data->no_w_space_line);
	data->splited_line = ft_split(data->no_w_space_line, ' ');
	if (!data->splited_line || !*data->splited_line)
		return (0);
	j = 0;
	while (data->splited_line[++j])
		if (ft_ismeta(data->splited_line[j][0]))
			if (ft_ismeta(data->splited_line[j-1][ft_strlen(data->splited_line[j-1]) - 1]))
				if (data->splited_line[j-1][ft_strlen(data->splited_line[j-1]) - 1] != '|')
					return (error(data, 2, data->splited_line[j][0]), 1);
	return (EXIT_SUCCESS);
}
//error if # |...   or #<<<.. / >>>..  or #><... / <>...  or #>ø / <ø  or  #...|ø / ||
int	check_syntax(t_data *data)
{
	char *ptr;

	data->no_space_line = ft_strdelspace(data->line); //pour une première vérification sans espaces
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
	return (check_space(data)); //et une deuxieme en splitant sur les white spaces
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
	return (in_s_quote || in_quote); // il faut qu'à la fin de la ligne, les deux soit FALSE / fermé
}
