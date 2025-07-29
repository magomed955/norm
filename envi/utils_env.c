/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:09:22 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/07/29 16:32:36 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	error_handling(int err, t_data *data)
{
	if (data)
		free_data(data);
	if (err == 6)
		perror("Error creating pipe");
	else if (err == 3)
	{
		perror("command not found");
		exit(127);
	}
	exit(EXIT_FAILURE);
}

void	free_tab(char **str)
{
	int	i;

	i = 0;
	if (!str)
		return ;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

char	*find_path_env(t_data *data)
{
	int	i;

	i = 0;
	while (data->env->env[i] && ft_strncmp(data->env->env[i], "PATH=", 5) != 0)
		i++;
	if (!data->env->env[i])
		return (NULL);
	return (data->env->env[i] + 5);
}

char	*build_full_path(char *dir, char *cmd)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(dir, "/");
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	return (full_path);
}

char	*get_path(char *cmd, t_data *data, char **argv)
{
	int		i;
	char	**path;
	char	*good_path;
	char	*path_env;

	(void)argv;
	path_env = find_path_env(data);
	if (!path_env)
		return (NULL);
	path = ft_split(path_env, ':');
	i = 0;
	while (path[i])
	{
		good_path = build_full_path(path[i], cmd);
		if (access(good_path, X_OK) == 0)
			return (free_tab(path), good_path);
		free(good_path);
		i++;
	}
	free_tab(path);
	return (NULL);
}
