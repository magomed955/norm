/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 22:57:01 by mubersan          #+#    #+#             */
/*   Updated: 2025/07/29 17:51:24 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_cmd(t_cmd *cmd)
{
	t_cmd	*tmp;

	while (cmd)
	{
		tmp = cmd->next;
		free_single_cmd(cmd);
		cmd = tmp;
	}
}

void	free_tokens(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token->next;
		free(token->content);
		free(token);
		token = tmp;
	}
}

void	free_env(t_env *env)
{
	int	i;

	i = 0;
	if (!env || !env->env)
		return ;
	while (env->env[i])
	{
		free(env->env[i]);
		i++;
	}
	free(env->env);
}

void	free_data(t_data *data)
{
	if (!data)
		return ;
	if (data->exit)
	{
		if (data->exit->exit)
			free(data->exit->exit);
		free(data->exit);
	}
	if (data->env)
	{
		free_env(data->env);
		free(data->env);
	}
	free(data);
}
