/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:23:22 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/07/29 18:24:05 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	add_token(t_token **head, char *content, int type)
{
	t_token	*new_token;
	t_token	*tmp;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return ;
	new_token->content = ft_strdup(content);
	new_token->type = type;
	new_token->next = NULL;
	new_token->prev = NULL;
	if (*head == NULL)
	{
		*head = new_token;
	}
	else
	{
		tmp = *head;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new_token;
		new_token->prev = tmp;
	}
}

void	handle_dollar_expansion(t_expand_ctx *ctx)
{
	char	*varname;
	char	*value;
	int		k;

	(*ctx->j)++;
	varname = get_dollar_value(ctx->prompt, *ctx->j);
	value = get_env_value(varname, ctx->data);
	k = 0;
	if (value)
	{
		while (value[k] && *ctx->i < BUFFER_SIZE - 1)
			ctx->buffer[(*ctx->i)++] = value[k++];
	}
	*ctx->j += ft_strlen(varname);
	free(varname);
	free(value);
}

int	handle_quotes(char *prompt, int *j, int *quote_state)
{
	if ((prompt[*j] == '\'' || prompt[*j] == '"') && *quote_state == 0)
	{
		update_quote_state(prompt[(*j)++], quote_state);
		return (1);
	}
	if ((prompt[*j] == '\'' && *quote_state == 1) || (prompt[*j] == '"'
			&& *quote_state == 2))
	{
		update_quote_state(prompt[(*j)++], quote_state);
		return (1);
	}
	return (0);
}

int	handle_redirections(t_token_ctx *ctx)
{
	if ((ctx->prompt[*ctx->j] == '<' && ctx->prompt[*ctx->j + 1] == '<')
		&& ctx->quote_state == 0)
		return (handle_heredoc(ctx->head, ctx->buffer, ctx->i, ctx->j), 1);
	if (ctx->prompt[*ctx->j] == '<' && ctx->quote_state == 0)
		return (handle_redin(ctx->head, ctx->buffer, ctx->i, ctx->j), 1);
	if ((ctx->prompt[*ctx->j] == '>' && ctx->prompt[*ctx->j + 1] == '>')
		&& ctx->quote_state == 0)
		return (handle_append(ctx->head, ctx->buffer, ctx->i, ctx->j), 1);
	if (ctx->prompt[*ctx->j] == '>' && ctx->quote_state == 0)
		return (handle_redout(ctx->head, ctx->buffer, ctx->i, ctx->j), 1);
	return (0);
}

int	handle_special_chars(t_token_ctx *ctx)
{
	if (ctx->prompt[*ctx->j] == ' ' && ctx->quote_state == 0)
		return (handle_space(ctx->head, ctx->buffer, ctx->i, ctx->j), 1);
	if (ctx->prompt[*ctx->j] == '&' && ctx->quote_state == 0)
	{
		if (ctx->prompt[*ctx->j + 1] == '&')
		{
			add_token(ctx->head, "&&", AND_IF);
			*ctx->j += 2;
			return (1);
		}
	}
	if (ctx->prompt[*ctx->j] == '|' && ctx->quote_state == 0)
	{
		if (ctx->prompt[*ctx->j + 1] == '|')
		{
			add_token(ctx->head, "||", OR_IF);
			*ctx->j += 2;
			return (1);
		}
		else
		{
			return (handle_pipe(ctx->head, ctx->buffer, ctx->i, ctx->j), 1);
		}
	}
	return (handle_redirections(ctx));
}
