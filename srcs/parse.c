#include "../includes/minishell.h"

char	**fill_cmd(t_data *data, char **begin)
{
	char	*ptr;
	char	**cmd;
	t_list	*node_ptr;
	int		j;

	ptr = *begin;
	while (*ptr && !ft_ismeta(*ptr))
	{
		while (ft_iswhitespace(*ptr))
			ptr++;
		ft_lstadd_back(&(data->cmd_param), ft_lstnew(str__dup(data, &ptr)));
	}
	*begin = ptr;
	cmd = malloc(sizeof(char *) * (ft_lstsize(data->cmd_param) + 1));
	if (!cmd)
		return (perror("Malloc"), close_free_exit(data, EXIT_FAILURE), NULL);
	node_ptr = data->cmd_param;
	j = -1;
	while (node_ptr)
	{
		cmd[++j] = node_ptr->content;
		node_ptr = node_ptr->next;
	}
	cmd[++j] = 0;
	return (cmd);
}

void	parse_cmd(t_data *data, t_list *node, char **begin)
{
	char	*ptr;

	ptr = *begin;
	while (*ptr && *ptr != '|')
	{
		if (*ptr == '>')
		{
			ptr++;
			if (*ptr == '>')
			{
				node->append_out = TRUE;
				ptr++;
			}
			else
				node->append_out = FALSE;
			while (ft_iswhitespace(*ptr))
				ptr++;
			free(node->file_out);
			node->file_out = str__dup(data, &ptr);
		}
		else if (*ptr == '<')
		{
			ptr++;
			if (*ptr == '<')
			{
				ptr++;
				while (ft_iswhitespace(*ptr))
					ptr++;
				free(node->lim);
				node->lim = str__dup(data, &ptr);
				continue;
			}
			while (ft_iswhitespace(*ptr))
				ptr++;
			free(node->file_in);
			node->file_in = str__dup(data, &ptr);
		}
		else if (!ft_iswhitespace(*ptr))
			node->cmd = fill_cmd(data, &ptr);
		while (ft_iswhitespace(*ptr))
			ptr++;
	}
	*begin = ptr;
}

void	parse(t_data *data)
{
	char	*ptr;
	t_list	*node;

	if (check_quote(data))
		return (error(data, 'q', 0));
	if (check_syntax(data))
		return;
	expand(data);
	ptr = data->line;
	while (*ptr)
	{
		while (ft_iswhitespace(*ptr))
			ptr++;
		if (*ptr)
		{
			node = ft_calloc(1, sizeof(t_list));
			ft_lstadd_back(&data->cmds, node);			
			parse_cmd(data, node, &ptr);
		}
		if (*ptr)
			ptr++;
	}
}
