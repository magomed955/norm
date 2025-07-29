/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_signals.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:15:38 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/07/29 19:26:16 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	setup_signals_parent(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_signals_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	restore_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	handle_wait_status(t_wait_ctx *ctx)
{
	if (ctx->finished_pid == ctx->last_pid)
	{
		if (WIFEXITED(ctx->status))
			*ctx->data->exit->exit = WEXITSTATUS(ctx->status);
		else if (WIFSIGNALED(ctx->status))
			*ctx->data->exit->exit = 128 + WTERMSIG(ctx->status);
		if (WIFSIGNALED(ctx->status) && WTERMSIG(ctx->status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		if (WIFSIGNALED(ctx->status) && WTERMSIG(ctx->status) == SIGQUIT)
			write(STDOUT_FILENO, "Quit\n", 5);
	}
}

void	wait_for_children(int count, pid_t last_pid, t_data *data)
{
	int			i;
	int			status;
	pid_t		finished_pid;
	t_wait_ctx	ctx;

	i = 0;
	while (i < count)
	{
		finished_pid = waitpid(-1, &status, 0);
		ctx = (t_wait_ctx){status, finished_pid, last_pid, data};
		handle_wait_status(&ctx);
		i++;
	}
}
