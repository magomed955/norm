/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:15:32 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/07/29 20:14:10 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	cleanup_and_exit(char **argv, t_data *data, int exit_code)
{
	if (data->fds)
		close_all_fds(data->fds, data->cmd_count);
	if (argv)
		free_array(argv);
	free_cmd(data->cmd);
	free_tokens(data->token);
	*data->exit->exit = exit_code;
	free_data(data);
	exit(exit_code);
}

void	clean_before_cmd(t_cmd *cmd_to_clean, t_cmd *cmd_final)
{
	t_cmd	*tmp;

	if (!cmd_to_clean || !cmd_final)
		return ;
	while (cmd_to_clean != cmd_final)
	{
		tmp = cmd_to_clean->next;
		free_single_cmd(cmd_to_clean);
		cmd_to_clean = tmp;
	}
}

void	clean_before_token(t_token *token_to_clean, t_token *token_final)
{
	t_token	*tmp;

	if (!token_to_clean || !token_final)
		return ;
	while (token_to_clean != token_final)
	{
		tmp = token_to_clean->next;
		free(token_to_clean->content);
		free(token_to_clean);
		token_to_clean = tmp;
	}
}

void	close_fds(void)
{
	int	fd;

	fd = 3;
	while (fd < 1024)
	{
		close(fd);
		fd++;
	}
}

void	cleanup_heredocs(t_cmd *head)
{
	t_cmd	*cleanup_cmd;

	cleanup_cmd = head;
	while (cleanup_cmd)
	{
		if (cleanup_cmd->heredoc_fd > 2)
		{
			close(cleanup_cmd->heredoc_fd);
			cleanup_cmd->heredoc_fd = -1;
		}
		cleanup_cmd = cleanup_cmd->next;
	}
}
