/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:34:00 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/07/29 22:00:30 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd	*init_struct(void)
{
	t_cmd	*new_cmd;

	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	new_cmd->cmd = NULL;
	new_cmd->args = NULL;
	new_cmd->infile = NULL;
	new_cmd->outfile = NULL;
	new_cmd->append = 0;
	new_cmd->redir_in = NULL;
	new_cmd->heredoc = NULL;
	new_cmd->nb_heredoc = 0;
	new_cmd->heredoc_fd = -1;
	new_cmd->next = NULL;
	return (new_cmd);
}

int	init_exit_data(t_data *data)
{
	data->exit = malloc(sizeof(t_exit));
	if (!data->exit)
		return (0);
	data->exit->exit = malloc(sizeof(int));
	if (!data->exit->exit)
	{
		free(data->exit);
		return (0);
	}
	*data->exit->exit = 0;
	return (1);
}

int	init_default_env(t_data *data)
{
	char	**default_env;

	default_env = create_default_env();
	if (!default_env)
		return (0);
	data->env->env = default_env;
	return (1);
}

int	init_env_data(t_data *data, char **envp)
{
	data->env = malloc(sizeof(t_env));
	if (!data->env)
		return (0);
    data->env->export_only = NULL;
	if (!envp || !envp[0])
	{
		if (!init_default_env(data))
		{
			free(data->env);
			return (0);
		}
	}
	else
	{
		copy_env(data, envp);
		if (!data->env->env)
		{
			free(data->env);
			return (0);
		}
	}
	return (1);
}

t_data	*init_data(char **envp)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	if (!init_exit_data(data))
		return (free(data), NULL);
	if (!init_env_data(data, envp))
	{
		free(data->exit->exit);
		free(data->exit);
		return (free(data), NULL);
	}
	update_shlvl(data);
	data->token = NULL;
	data->cmd = NULL;
	data->should_exit = 0;
	data->fds = NULL;
	data->cmd_count = 0;
	return (data);
}
