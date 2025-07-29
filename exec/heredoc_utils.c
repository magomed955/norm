/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:03:40 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/07/29 20:00:54 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	close_here_fd(int write_fd)
{
	int	fd;

	fd = 3;
	while (fd < 1024)
	{
		if (fd != write_fd)
			close(fd);
		fd++;
	}
}

void	allocate_heredoc(t_token *token, t_cmd *cmd)
{
	t_cmd	*current;

	current = cmd;
	while (token && current)
	{
		if (token->type == HEREDOC)
		{
			current->nb_heredoc++;
		}
		else if (token->type == PIPE)
		{
			current = current->next;
		}
		token = token->next;
	}
	current = cmd;
	while (current)
	{
		if (current->nb_heredoc > 0)
			current->heredoc = malloc(sizeof(char *) * (current->nb_heredoc
						+ 1));
		if (current->heredoc)
			current->heredoc[current->nb_heredoc] = NULL;
		current = current->next;
	}
}

void	process_heredocs(t_cmd *cmd, t_data *data)
{
	t_cmd	*tmp;

	tmp = cmd;
	while (tmp)
	{
		if (tmp->nb_heredoc > 0)
		{
			if (create_heredoc_pipe(tmp, data) == -1)
				error_handling(3, data);
		}
		tmp = tmp->next;
	}
}

int	handle_no_command(t_cmd *cmds, t_data *data, t_token *tokens, char *prompt)
{
	t_cmd	*check;

	check = cmds;
	(void)prompt;
	while (check)
	{
		if (check->cmd != NULL)
			return (0);
		check = check->next;
	}
	free_cmd(cmds);
	*data->exit->exit = 0;
	free_tokens(tokens);
	return (1);
}

void	handle_heredoc_eof(char *delimiter)
{
	ft_putstr_fd(
		"bash: warning: here-document delimited by end-of-file (wanted `",
		2);
	ft_putstr_fd(delimiter, 2);
	ft_putstr_fd("')\n", 2);
}
