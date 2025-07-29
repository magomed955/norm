/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:30:39 by mubersan          #+#    #+#             */
/*   Updated: 2025/07/29 19:30:24 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_directory_error(char **argv, t_data *data)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(argv[0], 2);
	ft_putstr_fd(": Is a directory\n", 2);
	cleanup_and_exit(argv, data, 126);
}

void	handle_command_not_found(char **argv, t_data *data)
{
	ft_putstr_fd(argv[0], 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd("command not found\n", 2);
	if (argv)
		free_tab(argv);
	cleanup_and_exit(NULL, data, 127);
}

void	close_unused_fds(t_exec_context *ctx)
{
	int	i;

	if (!ctx || !ctx->fds)
		return ;
	i = 0;
	while (i < (ctx->total_cmds - 1) * 2)
		close(ctx->fds[i++]);
}

void	handle_empty_argv(char **argv, t_data *data)
{
	if (!argv || !argv[0])
		cleanup_and_exit(argv, data, 0);
}

void	handle_execve_error(char **argv)
{
	if (errno == EACCES)
	{
		ft_putstr_fd(argv[0], 2);
		ft_putstr_fd(": Permission denied\n", 2);
	}
	else if (errno == ENOEXEC)
	{
		ft_putstr_fd(argv[0], 2);
		ft_putstr_fd(": cannot execute binary file: Exec format error\n", 2);
	}
	else if (errno == ENOENT)
	{
		ft_putstr_fd(argv[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
	}
	else if (errno == EISDIR)
	{
		ft_putstr_fd(argv[0], 2);
		ft_putstr_fd(": Is a directory\n", 2);
	}
	else
		perror("execve failed");
}
