/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:15:37 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/07/29 19:45:55 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	redirect_input_file(t_cmd *cmd, t_redir_in *last)
{
	if (last->type == REDIN)
		return (handle_redirect(last));
	else if (last->type == HEREDOC && cmd->heredoc_fd > 2)
	{
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	return (1);
}

int	redirect_input(t_cmd *cmd, int *fds, int index)
{
	t_redir_in	*last;
	t_redir_in	*tmp;

	last = NULL;
	tmp = cmd->redir_in;
	while (tmp)
	{
		last = tmp;
		tmp = tmp->next;
	}
	if (last)
		return (redirect_input_file(cmd, last));
	else if (index > 0 && fds)
	{
		dup2(fds[(index - 1) * 2], STDIN_FILENO);
		close(fds[(index - 1) * 2]);
	}
	return (1);
}

int	redirect_output(t_cmd *cmd, int *fds, int index, int is_last)
{
	int	fd;

	if (cmd->outfile != NULL)
	{
		if (!validate_outfile(cmd->outfile))
			return (0);
		if (cmd->append == 0)
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
		else
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0777);
		if (fd == -1)
		{
			dprintf(2, "minishell: %s: No such file or directory\n",
				cmd->outfile);
			return (0);
		}
		dup2(fd, 1);
		close(fd);
	}
	else if (!is_last)
	{
		dup2(fds[index * 2 + 1], 1);
		close(fds[index * 2 + 1]);
	}
	return (1);
}

int	validate_outfile(char *outfile)
{
	struct stat	stats;

	if (!access(outfile, F_OK))
	{
		if (access(outfile, W_OK) == -1)
		{
			dprintf(2, "minishell: %s: ", outfile);
			perror("");
			return (0);
		}
		if (!stat(outfile, &stats))
		{
			if (stats.st_mode & S_IFDIR)
			{
				dprintf(2, "minishell: %s: Is a directory\n", outfile);
				return (0);
			}
		}
	}
	return (1);
}
