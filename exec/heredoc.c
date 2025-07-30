/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 20:31:59 by mubersan          #+#    #+#             */
/*   Updated: 2025/07/30 17:01:52 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_heredoc_parent(pid_t pid, int *fds, t_data *data)
{
	int	status;

	close(fds[1]);
	waitpid(pid, &status, 0);
	enable_echoctl();
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	if (WIFEXITED(status))
	{
		*data->exit->exit = 130;
		return (0);
	}
	return (0);
}

int	setup_heredoc_pipe(int *fds)
{
	if (pipe(fds) == -1)
		return (-1);
	return (0);
}

pid_t	fork_heredoc_process(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	return (pid);
}

int	create_heredoc_pipe(t_cmd *cmd, t_data *data)
{
	int		fds[2];
	pid_t	pid;

	if (setup_heredoc_pipe(fds) == -1)
		return (-1);
	pid = fork_heredoc_process();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		close(fds[0]);
		heredoc_child_process(cmd, fds[1], data);
	}
	if (handle_heredoc_parent(pid, fds, data) == 1)
		return (1);
	cmd->heredoc_fd = fds[0];
	return (0);
}
