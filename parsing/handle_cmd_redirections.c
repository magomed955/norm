/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_redirections.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:36:38 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/07/29 18:40:31 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_append_cmd(t_cmd *cmd, t_token *token)
{
	int	fd;

	if (!token || !token->next)
		return ;
	fd = open(token->next->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd != -1)
		close(fd);
	if (cmd->outfile)
		free(cmd->outfile);
	cmd->outfile = ft_strdup(token->next->content);
	cmd->append = 1;
}

void	handle_redout_cmd(t_cmd *cmd, t_token *token)
{
	int	fd;

	if (!token || !token->next)
		return ;
	if (cmd->outfile)
	{
		free(cmd->outfile);
		cmd->outfile = NULL;
	}
	fd = open(token->next->content, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd != -1)
		close(fd);
	cmd->outfile = ft_strdup(token->next->content);
	cmd->append = 0;
}

int	open_outfile(char *filename, int append)
{
	int	result;

	result = 0;
	if (append == 0)
		result = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	else
		result = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (result == -1)
		return (-1);
	return (result);
}

void	add_redir_in(t_cmd *cmd, int type, const char *value)
{
	t_redir_in	*new;
	t_redir_in	*tmp;

	new = malloc(sizeof(t_redir_in));
	if (!new)
		return ;
	new->type = type;
	new->value = ft_strdup(value);
	new->next = NULL;
	if (!cmd->redir_in)
	{
		cmd->redir_in = new;
		return ;
	}
	tmp = cmd->redir_in;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	fill_heredocs_from_tokens(t_token *token, t_cmd *cmd)
{
	int	i;

	i = 0;
	while (token && cmd)
	{
		if (token->type == HEREDOC && token->next)
		{
			cmd->heredoc[i] = ft_strdup(token->next->content);
			i++;
		}
		else if (token->type == PIPE)
		{
			cmd = cmd->next;
			i = 0;
		}
		token = token->next;
	}
}
