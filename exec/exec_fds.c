/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:15:33 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/07/29 19:49:58 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	create_fds(t_cmd *cmd, int **fds_out)
{
	int	count;
	int	*fds;

	count = count_all_commands(cmd);
	if (count <= 1)
	{
		*fds_out = NULL;
		return (count);
	}
	fds = malloc(sizeof(int) * 2 * (count - 1));
	if (!fds)
		return (-1);
	if (create_pipes(fds, count - 1) == -1)
	{
		free(fds);
		return (-1);
	}
	*fds_out = fds;
	return (count);
}

int	create_pipes(int *fds, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (pipe(&fds[i * 2]) == -1)
		{
			free(fds);
			return (-1);
		}
		i++;
	}
	return (0);
}

int	setup_execution_context(t_cmd *cmd, t_data *data, int **fds)
{
	int	count;

	count = count_all_commands(cmd);
	*fds = NULL;
	if (count > 0 && create_fds(cmd, fds) == -1)
		error_handling(6, data);
	data->fds = *fds;
	data->cmd_count = count;
	setup_signals_parent();
	return (count);
}

int	execute_single_command_if_needed(t_cmd *cmd, t_pipeline_ctx *ctx)
{
	t_exec_context	exec_ctx;
	char			**argv;

	argv = build_argv(cmd);
	if (!argv || !argv[0] || is_builtin(argv[0]) || is_parent_builtin(argv[0]))
	{
		exec_ctx = (t_exec_context){ctx->fds, 0, 1, 1};
		execute_single_builtin(cmd, &exec_ctx, ctx->data, argv);
		free_array(argv);
		return (1);
	}
	free_array(argv);
	return (0);
}
