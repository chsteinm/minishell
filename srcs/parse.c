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
		ft_skip_wspaces(&ptr);
		ft_lstadd_back(&(data->cmd_param), ft_lstnew(str__dup(data, &ptr)));
		ft_skip_wspaces(&ptr);
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

void	parse_in(t_data *data, t_list *node, char **begin)
{
	char	*ptr;

	ptr = *begin;
	if (*ptr == '<')
	{
		ptr++;
		ft_skip_wspaces(&ptr);
		free(node->lim);
		node->lim = str__dup(data, &ptr);
	}
	else
	{
		ft_skip_wspaces(&ptr);
		free(node->file_in);
		node->file_in = str__dup(data, &ptr);
		open_in(data, node, &ptr);
	}
	*begin = ptr;
}

void	parse_out(t_data *data, t_list *node, char **begin)
{
	char	*ptr;

	ptr = *begin;
	if (*ptr == '>')
	{
		node->append_out = TRUE;
		ptr++;
	}
	else
		node->append_out = FALSE;
	ft_skip_wspaces(&ptr);
	free(node->file_out);
	node->file_out = str__dup(data, &ptr);
	open_out(data, node, &ptr);
	*begin = ptr;
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
			parse_out(data, node, &ptr);
		}
		else if (*ptr == '<')
		{
			ptr++;
			parse_in(data, node, &ptr);
		}
		else if (!ft_iswhitespace(*ptr))
			node->cmd = fill_cmd(data, &ptr);
		ft_skip_wspaces(&ptr);
	}
	*begin = ptr;
}

void	parse(t_data *data)
{
	char	*ptr;
	t_list	*node;

	if (check_quote(data))
		return (error(data, 1, 'q'));
	if (check_syntax(data))
		return;
	expand(data); //les modifications se font directement sur la ligne renvoyer par readline (data->line)
	ptr = data->line;
	while (*ptr)
	{
		ft_skip_wspaces(&ptr);
		if (*ptr)
		{
			node = ft_calloc(1, sizeof(t_list));
			if (!node)
				return (perror("Malloc"), close_free_exit(data, EXIT_FAILURE));
			ft_lstadd_back(&data->cmds, node);			
			parse_cmd(data, node, &ptr);
			ft_lstclear(&data->cmd_param, NULL);
		}
		if (*ptr)
			ptr++;
	}
}
