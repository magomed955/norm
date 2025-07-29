/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 20:47:19 by mubersan          #+#    #+#             */
/*   Updated: 2025/07/29 18:31:35 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_tokens(t_token *head)
{
	t_token	*tmp;

	tmp = head;
	while (tmp)
	{
		printf("Content: %s | Type: %d\n", tmp->content, tmp->type);
		tmp = tmp->next;
	}
}

void	init_tokenize_vars(t_tokenize_vars *vars, t_token **head)
{
	vars->head = head;
	vars->i = 0;
	vars->j = 0;
	vars->quote_state = 0;
	*head = NULL;
}

int	process_token_char(char *prompt, t_data *data, t_tokenize_vars *vars)
{
	t_token_ctx		ctx;
	t_expand_ctx	expand_ctx;

	(void)data;
	ctx.prompt = prompt;
	ctx.head = vars->head;
	ctx.buffer = vars->buffer;
	ctx.i = &vars->i;
	ctx.j = &vars->j;
	ctx.quote_state = vars->quote_state;
	if (handle_quotes(prompt, &vars->j, &vars->quote_state))
		return (1);
	if (handle_special_chars(&ctx))
		return (1);
	if (prompt[vars->j] == '$' && vars->quote_state != 1)
	{
		expand_ctx = (t_expand_ctx){prompt, data, vars->buffer, &vars->i,
			&vars->j};
		handle_dollar_expansion(&expand_ctx);
	}
	else
		vars->buffer[vars->i++] = prompt[vars->j++];
	return (0);
}

int	finalize_tokenize(t_tokenize_vars *vars)
{
	if (vars->quote_state != 0)
	{
		ft_putstr_fd("syntax error: unclosed quote\n", 2);
		return (0);
	}
	if (vars->i > 0)
	{
		vars->buffer[vars->i] = '\0';
		add_token(vars->head, vars->buffer, WORD);
	}
	return (1);
}

t_token	*tokenize(char *prompt, t_data *data)
{
	t_token			*head;
	char			buffer[BUFFER_SIZE];
	t_tokenize_vars	vars;

	init_tokenize_vars(&vars, &head);
	vars.buffer = buffer;
	while (prompt[vars.j])
	{
		if (process_token_char(prompt, data, &vars))
			continue ;
	}
	if (!finalize_tokenize(&vars))
	{
		free_tokens(head);
		return (NULL);
	}
	if (!validate_syntax(head))
	{
		free_tokens(head);
		return (NULL);
	}
	return (head);
}
