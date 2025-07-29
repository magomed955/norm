/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_messages.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 09:26:57 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/07/29 14:45:07 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_operator(int type)
{
	return (type == PIPE || type == OR_IF || type == AND_IF
		|| type == REDOUT || type == APPEND
		|| type == REDIN || type == HEREDOC);
}

int	is_redirection(int type)
{
	return (type == REDOUT || type == APPEND
		|| type == REDIN || type == HEREDOC);
}

int	has_valid_command(t_token *tokens)
{
	t_token	*cur;

	cur = tokens;
	while (cur)
	{
		if (cur->type == WORD
			&& (!cur->prev || !is_redirection(cur->prev->type)))
			return (1);
		cur = cur->next;
	}
	return (0);
}

int	print_syntax_error(t_token *token)
{
	if (!token)
		ft_putstr_fd("bash: syntax error near unexpected token `newline'\n", 2);
	else if (token->type == PIPE)
		ft_putstr_fd("bash: syntax error near unexpected token `|'\n", 2);
	else if (token->type == OR_IF)
		ft_putstr_fd("bash: syntax error near unexpected token `||'\n", 2);
	else if (token->type == AND_IF)
		ft_putstr_fd("bash: syntax error near unexpected token `&&'\n", 2);
	else if (token->type == APPEND)
		ft_putstr_fd("bash: syntax error near unexpected token `>>'\n", 2);
	else if (token->type == HEREDOC)
		ft_putstr_fd("bash: syntax error near unexpected token `<<'\n", 2);
	else if (token->type == READWRITE)
		ft_putstr_fd("bash: syntax error near unexpected token `<>'\n", 2);
	else if (token->type == REDOUT || token->type == REDIN)
		ft_putstr_fd("bash: syntax error near unexpected token `newline'\n", 2);
	else
		ft_putstr_fd("bash: syntax error near unexpected token\n", 2);
	return (0);
}

int	validate_syntax(t_token *tokens)
{
	t_token	*cur;

	cur = tokens;
	if (!tokens)
		return (1);
	if (cur->type == PIPE || cur->type == AND_IF || cur->type == OR_IF)
		return (print_syntax_error(cur), 0);
	while (cur)
	{
		if (is_operator(cur->type))
		{
			if (!cur->next)
				return (print_syntax_error(NULL), 0);
			if (is_operator(cur->next->type))
				return (print_syntax_error(cur->next), 0);
		}
		cur = cur->next;
	}
	if (!has_valid_command(tokens))
	{
		ft_putstr_fd("bash: syntax error near unexpected token `newline'\n", 2);
		return (0);
	}
	return (1);
}
