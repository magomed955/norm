/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:15:30 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/07/29 19:48:53 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_builtin_execution(char **argv, t_data *data)
{
	if (is_builtin(argv[0]) || is_parent_builtin(argv[0]))
	{
		exec_builtin(argv, data);
		cleanup_and_exit(argv, data, *data->exit->exit);
	}
}

void	execute_single_builtin(t_cmd *cmd, t_exec_context *ctx,
		t_data *data, char **argv)
{
	int	stdin_fd;
	int	stdout_fd;

	stdin_fd = dup(STDIN_FILENO);
	stdout_fd = dup(STDOUT_FILENO);
	if (!redirect_input(cmd, NULL, ctx->index))
	{
		reset_fds(stdin_fd, stdout_fd);
		*data->exit->exit = 1;
		return ;
	}
	if (!redirect_output(cmd, NULL, ctx->index, 1))
	{
		reset_fds(stdin_fd, stdout_fd);
		*data->exit->exit = 1;
		return ;
	}
	if (argv && argv[0])
		exec_builtin(argv, data);
	reset_fds(stdin_fd, stdout_fd);
}

void	execute_all_cmd(t_cmd *cmd, t_data *data, t_token *tokens)
{
	int				count;
	int				*fds;
	t_cmd			*head;
	t_pipeline_ctx	ctx;

	head = cmd;
	count = setup_execution_context(cmd, data, &fds);
	ctx = (t_pipeline_ctx){data, tokens, fds, count};
	execute_pipeline(cmd, &ctx);
	cleanup_heredocs(head);
	restore_signals();
}

int	handle_redirect(t_redir_in *last)
{
	int	fd;

	if (access(last->value, F_OK | R_OK) == -1)
	{
		dprintf(2, "minishell: %s: ", last->value);
		perror("");
		return (0);
	}
	fd = open(last->value, O_RDONLY);
	if (fd == -1)
	{
		dprintf(2, "minishell: %s: No such file or directory\n",
			last->value);
		return (0);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (1);
}

pid_t	execute_pipeline_loop(t_cmd *current, t_pipeline_ctx *ctx)
{
	pid_t			last_pid;
	t_exec_context	exec_ctx;
	int				index;

	last_pid = -1;
	index = 0;
	while (current)
	{
		exec_ctx = (t_exec_context){ctx->fds, index, (index == ctx->count - 1),
			ctx->count};
		if (exec_ctx.is_last)
			last_pid = fork_and_execute(current, &exec_ctx, ctx->data,
					ctx->tokens);
		else
			fork_and_execute(current, &exec_ctx, ctx->data, ctx->tokens);
		close_pipe_fds(ctx->fds, index, ctx->count);
		index++;
		current = current->next;
	}
	return (last_pid);
}
