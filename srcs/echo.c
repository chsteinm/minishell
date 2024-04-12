void	ft_echo(t_data *data, t_list *node)
{
	int	i;

	i = 1;
	while (node->cmd[i])
	{
		ft_putstr_fd(node->cmd[i], 1);
		if (node->cmd[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (node->cmd[1])
		ft_putstr_fd("\n", 1);
	data->ret = 0;
}
