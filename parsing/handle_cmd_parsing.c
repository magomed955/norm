/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_parsing.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:39:26 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/07/29 18:50:08 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_pipe_cmd(t_cmd **current_cmd)
{
	t_cmd	*new_cmd;

	new_cmd = init_struct();
	(*current_cmd)->next = new_cmd;
	*current_cmd = new_cmd;
}

int	handle_redirection(t_cmd *current_cmd, t_token **token)
{
	if (!(*token)->next)
		return (0);
	if ((*token)->type == REDIN)
		add_redir_in(current_cmd, REDIN, (*token)->next->content);
	else if ((*token)->type == REDOUT)
		handle_redout_cmd(current_cmd, *token);
	else if ((*token)->type == APPEND)
		handle_append_cmd(current_cmd, *token);
	else if ((*token)->type == HEREDOC)
		add_redir_in(current_cmd, HEREDOC, (*token)->next->content);
	else
		return (0);
	*token = (*token)->next;
	return (1);
}

void	process_token(t_cmd **current_cmd, t_token **token)
{
	if ((*token)->type == WORD)
		handle_word_cmd(*current_cmd, *token);
	else if ((*token)->type == PIPE)
		handle_pipe_cmd(current_cmd);
	else if ((*token)->type == REDIN || (*token)->type == REDOUT
		|| (*token)->type == APPEND || (*token)->type == HEREDOC)
	{
		if (!handle_redirection(*current_cmd, token))
			*token = NULL;
	}
}

t_cmd	*parse_tokens(t_token *token)
{
	t_cmd	*head;
	t_cmd	*current_cmd;

	if (!token)
		return (NULL);
	head = init_struct();
	if (!head)
		return (NULL);
	current_cmd = head;
	while (token != NULL)
	{
		process_token(&current_cmd, &token);
		if (!token)
			return (NULL);
		token = token->next;
	}
	return (head);
}
