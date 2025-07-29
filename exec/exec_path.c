/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:15:36 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/07/29 19:23:09 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*prepare_path(char *cmd, t_data *data)
{
	char	*path;
	char	**argv;

	if (!cmd || !cmd[0])
		return (NULL);
	argv = ft_split(cmd, ' ');
	if (!argv)
		return (NULL);
	if (access(argv[0], F_OK) == 0)
	{
		if (is_directory(argv[0]))
			return (free_tab(argv), NULL);
		path = ft_strdup(argv[0]);
	}
	else if (data->env)
		path = get_path(argv[0], data, argv);
	else
		return (free_tab(argv), NULL);
	if (!path)
		return (free_tab(argv), NULL);
	free_tab(argv);
	return (path);
}

int	is_directory(char *path)
{
	struct stat	stat_buf;

	if (stat(path, &stat_buf) == 0 && S_ISDIR(stat_buf.st_mode))
		return (1);
	return (0);
}

int	is_directory_path(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "/") == 0)
		return (1);
	if (ft_strcmp(cmd, "./") == 0)
		return (1);
	if (ft_strcmp(cmd, "../") == 0)
		return (1);
	return (0);
}
