/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:05:06 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/07/30 17:01:13 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	process_heredoc_line(char *line, char *delimiter, int fd, int is_last)
{
	if (line[ft_strlen(line) - 1] == '\n')
		line[ft_strlen(line) - 1] = '\0';
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (1);
	}
	if (is_last)
	{
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
	}
	free(line);
	return (0);
}

void	process_single_heredoc(t_cmd *cmd, int write_fd, int index,
		t_data *data)
{
	char	*line;

	while (1)
	{
		ft_putstr_fd("> ", 1);
		line = get_next_line(0);
		if (g_exit_status == 1)
		{
			close(write_fd);
			cleanup_and_exit(NULL, data, 130);
		}
		if (!line)
		{
			handle_heredoc_eof(cmd->heredoc[index]);
			break ;
		}
		if (process_heredoc_line(line, cmd->heredoc[index], write_fd,
				(index == cmd->nb_heredoc - 1)))
			break ;
	}
}

void	heredoc_child_process(t_cmd *cmd, int write_fd, t_data *data)
{
	int	i;

	handle_sigint_heredoc();
	close_here_fd(write_fd);
	i = 0;
	while (i < cmd->nb_heredoc)
	{
		process_single_heredoc(cmd, write_fd, i, data);
		i++;
	}
	close(write_fd);
	cleanup_and_exit(NULL, data, 0);
}
