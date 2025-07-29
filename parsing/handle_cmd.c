/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 23:17:09 by mubersan          #+#    #+#             */
/*   Updated: 2025/07/29 18:57:25 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_first_arg(t_cmd *cmd, t_token *token)
{
	cmd->args = malloc(sizeof(char *) * 2);
	if (!cmd->args)
		return ;
	cmd->args[0] = ft_strdup(token->content);
	cmd->args[1] = NULL;
}

void	add_arg_to_existing(t_cmd *cmd, t_token *token)
{
	int		i;
	char	**tmp;
	int		j;

	i = 0;
	while (cmd->args[i])
		i++;
	tmp = malloc(sizeof(char *) * (i + 2));
	if (!tmp)
		return ;
	j = 0;
	while (j < i)
	{
		tmp[j] = cmd->args[j];
		j++;
	}
	tmp[j] = ft_strdup(token->content);
	tmp[j + 1] = NULL;
	free(cmd->args);
	cmd->args = tmp;
}

void	handle_word_cmd(t_cmd *cmd, t_token *token)
{
	if (cmd->cmd == NULL)
	{
		cmd->cmd = ft_strdup(token->content);
	}
	else
	{
		if (cmd->args == NULL)
			init_first_arg(cmd, token);
		else
			add_arg_to_existing(cmd, token);
	}
}

char	**build_argv(t_cmd *cmd)
{
	int		count;
	char	**argv;

	if (!cmd)
	{
		argv = (char **)malloc(sizeof(char *));
		if (!argv)
			return (NULL);
		*argv = NULL;
		return (argv);
	}
	count = count_args(cmd->args);
	count++;
	argv = fill_argv(cmd, count);
	return (argv);
}
