/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:15:34 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/07/29 23:14:24 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

pid_t	fork_and_execute(t_cmd *cmd, t_exec_context *ctx, t_data *data,
		t_token *tokens)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		setup_signals_child();
		execute_one_cmd(cmd, ctx, data, tokens);
		exit(0);
	}
	return (pid);
}

void	handle_command_execution(char **argv, t_data *data)
{
	char	*path;

	if (is_directory_path(argv[0]))
		handle_directory_error(argv, data);
	path = prepare_path(argv[0], data);
	if (!path)
		handle_command_not_found(argv, data);
	execute_cmd(path, argv, data);
	cleanup_and_exit(argv, data, 126);
}

void	execute_cmd(char *path, char **argv, t_data *data)
{
	if (execve(path, argv, data->env->env) == -1)
	{
		handle_execve_error(argv);
		free(path);
		cleanup_and_exit(argv, data, 126);
	}
}

static void close_cfd(t_cmd *cmd, t_data *data) {
  int fd;
  
  (void)cmd;
  (void)data;
  fd = 3;
  while (fd < 1024) {
    close(fd);
    fd++;
  }
}

void	execute_one_cmd(t_cmd *cmd, t_exec_context *ctx, t_data *data,
		t_token *tokens)
{
	char	**argv;

	clean_before_token(data->token, tokens);
	clean_before_cmd(data->cmd, cmd);
	data->token = tokens;
	data->cmd = cmd;
	if (!redirect_input(cmd, ctx->fds, ctx->index))
	{
		close_fds();
		cleanup_and_exit(NULL, data, 1);
	}
	if (!redirect_output(cmd, ctx->fds, ctx->index, ctx->is_last))
		cleanup_and_exit(NULL, data, 1);
	close_unused_fds(ctx);
	close_cfd(cmd, data);
	argv = build_argv(cmd);
	handle_empty_argv(argv, data);
	handle_builtin_execution(argv, data);
	handle_command_execution(argv, data);
	cleanup_and_exit(argv, data, 0);
}

void	execute_pipeline(t_cmd *cmd, t_pipeline_ctx *ctx)
{
	pid_t	last_pid;
	t_cmd	*current;

	current = cmd;
	ctx->data->cmd = cmd;
	if (ctx->count == 1)
		if (execute_single_command_if_needed(cmd, ctx))
			return ;
	last_pid = execute_pipeline_loop(current, ctx);
	close_all_fds(ctx->fds, ctx->count);
	wait_for_children(ctx->count, last_pid, ctx->data);
}
