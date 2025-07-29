/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 17:50:45 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/07/29 17:51:29 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	free_redir_in_list(t_redir_in *redir)
{
	t_redir_in	*tmp;

	while (redir)
	{
		tmp = redir->next;
		if (redir->value)
			free(redir->value);
		free(redir);
		redir = tmp;
	}
}

static void	free_cmd_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

static void	free_cmd_heredoc(char **heredoc, int nb_heredoc)
{
	int	i;

	if (!heredoc)
		return ;
	i = 0;
	while (i < nb_heredoc && heredoc[i])
	{
		free(heredoc[i]);
		i++;
	}
	free(heredoc);
}

void	free_single_cmd(t_cmd *cmd)
{
	if (cmd->cmd)
		free(cmd->cmd);
	free_cmd_args(cmd->args);
	if (cmd->infile)
		free(cmd->infile);
	if (cmd->outfile)
		free(cmd->outfile);
	free_cmd_heredoc(cmd->heredoc, cmd->nb_heredoc);
	free_redir_in_list(cmd->redir_in);
	free(cmd);
}
