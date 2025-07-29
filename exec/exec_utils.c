/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:15:39 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/07/29 19:31:06 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	close_pipe_fds(int *fds, int index, int count)
{
	if (index > 0)
		close(fds[(index - 1) * 2]);
	if (index < count - 1)
		close(fds[index * 2 + 1]);
}

void	close_all_fds(int *fds, int count)
{
	if (!fds || count <= 1)
		return ;
	if (fds)
		free(fds);
}

int	*malloc_fds(int n)
{
	int	*fds;

	fds = malloc(sizeof(*fds) * 2 * (n - 1));
	if (fds == NULL)
		return (NULL);
	return (fds);
}

int	count_all_commands(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

void	reset_fds(int stdin_fd, int stdout_fd)
{
	dup2(stdin_fd, STDIN_FILENO);
	dup2(stdout_fd, STDOUT_FILENO);
	close(stdin_fd);
	close(stdout_fd);
}
