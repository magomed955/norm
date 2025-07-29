/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:46:43 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/07/29 19:01:38 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_args(char **args)
{
	int	count;

	count = 0;
	while (args && args[count])
		count++;
	return (count);
}

char	**fill_argv(t_cmd *cmd, int count)
{
	char	**argv;
	int		i;
	int		j;

	argv = (char **)malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	i = 0;
	argv[i++] = ft_strdup(cmd->cmd);
	j = 0;
	while (cmd->args && cmd->args[j])
	{
		argv[i++] = ft_strdup(cmd->args[j]);
		j++;
	}
	argv[i] = NULL;
	return (argv);
}

char	**create_default_env(void)
{
	char	cwd[BUFFER_SIZE];
	char	**default_env;

	if (!getcwd(cwd, BUFFER_SIZE))
		return (NULL);
	default_env = (char **)malloc(sizeof(char *) * 4);
	if (!default_env)
		return (NULL);
	default_env[0] = ft_strjoin("PWD=", cwd);
	default_env[1] = ft_strdup("SHLVL=0");
	default_env[2] = ft_strdup("_=/usr/bin/env");
	default_env[3] = NULL;
	if (!default_env[0] || !default_env[1] || !default_env[2])
	{
		if (default_env[0])
			free(default_env[0]);
		if (default_env[1])
			free(default_env[1]);
		if (default_env[2])
			free(default_env[2]);
		free(default_env);
		return (NULL);
	}
	return (default_env);
}
