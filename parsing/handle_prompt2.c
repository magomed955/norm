/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_prompt_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:20:27 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/07/29 18:20:42 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_pipe(t_token **head, char buffer[BUFFER_SIZE], int *i, int *j)
{
	if (*i > 0)
	{
		buffer[*i] = '\0';
		add_token(head, buffer, WORD);
		*i = 0;
	}
	add_token(head, "|", PIPE);
	(*j)++;
}

void	handle_space(t_token **head, char buffer[BUFFER_SIZE], int *i, int *j)
{
	if (*i > 0)
	{
		buffer[*i] = '\0';
		add_token(head, buffer, WORD);
		*i = 0;
	}
	(*j)++;
}
