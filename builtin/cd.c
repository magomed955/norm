/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:40:03 by mubersan          #+#    #+#             */
/*   Updated: 2025/07/29 17:20:04 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*backup_current_dir(void)
{
	char	cwd[BUFFER_SIZE];
	char	*oldpwd;

	if (get_current_dir(cwd) != 0)
		return (NULL);
	oldpwd = ft_strdup(cwd);
	if (!oldpwd)
		perror("cd: malloc");
	return (oldpwd);
}

int	change_directory(char *path)
{
	struct stat	buf;

	if (!stat(path, &buf) && !(buf.st_mode & S_IFDIR))
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Not a directory\n", 2);
		return (1);
	}
	if (stat(path, &buf) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		perror(": ");
		return (1);
	}
	if (chdir(path) != 0)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		return (1);
	}
	return (0);
}

int	validate_cd_args(char **args, t_data *data)
{
	if (!args)
		return (0);
	if (args[0] && args[1] && args[2])
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		*data->exit->exit = 1;
		return (0);
	}
	return (1);
}

char	*get_cd_path(char **args, t_data *data)
{
	char	*path;

	if (!args[1])
	{
		path = ft_getenv(data->env->env, "HOME");
		if (!path)
			ft_putstr_fd("cd: HOME not set\n", 2);
		return (path);
	}
	if (ft_strcmp(args[1], "-") == 0)
	{
		path = ft_getenv(data->env->env, "OLDPWD");
		if (!path)
			ft_putstr_fd("cd: OLDPWD not set\n", 2);
		else
			ft_putendl_fd(path, 1);
		return (path);
	}
	return (args[1]);
}

void	ft_cd(char **args, t_data *data)
{
	char	*oldpwd;
	char	*path;

	if (!validate_cd_args(args, data))
		return ;
	oldpwd = backup_current_dir();
	if (!oldpwd)
	{
		*data->exit->exit = 1;
		return ;
	}
	path = get_cd_path(args, data);
	if (!path || change_directory(path) != 0 || update_pwd(data, oldpwd) != 0)
		*data->exit->exit = 1;
	else
		*data->exit->exit = 0;
	free(oldpwd);
}
