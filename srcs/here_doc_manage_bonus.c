#include "../includes/pipex_bonus.h"

void	write_until_lim(t_data *data)
{
	char	*line;

	while (1)
	{
		line = get_next_line(data->fd_in);
		if (line)
		{
			if (*line != '\n')
				if (!ft_strncmp(data->limiter, line, ft_strlen(line) - 1))
					return (free(line));
			if (write(data->fildes_hd[1], line, ft_strlen(line)) == -1)
			{
				perror("write");
				close_free_exit(data, EXIT_FAILURE);
			}
			free(line);
		}
	}
}

void	here_doc_manage(t_data *data)
{
	if (data->fd_in == -1)
	{
		data->is_here_doc = 0;
		close_free_exit(data, EXIT_FAILURE);
	}
	if (pipe(data->fildes_hd) == -1)
	{
		perror("pipe");
		data->is_here_doc = 0;
		close_free_exit(data, EXIT_FAILURE);
	}
	write_until_lim(data);
	close(data->fd_in);
	data->fd_in = data->fildes_hd[0];
}