/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 18:54:11 by mubersan          #+#    #+#             */
/*   Updated: 2025/07/29 16:31:22 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_env_vars(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

void	cleanup_env_allocation(char **env, int up_to)
{
	while (up_to-- > 0)
		free(env[up_to]);
	free(env);
}

int	allocate_env_copy(t_data *data, char **envp, int count)
{
	int	j;

	j = 0;
	while (j < count)
	{
		data->env->env[j] = ft_strdup(envp[j]);
		if (!data->env->env[j])
		{
			cleanup_env_allocation(data->env->env, j);
			data->env->env = NULL;
			return (0);
		}
		j++;
	}
	data->env->env[j] = NULL;
	return (1);
}

void	copy_env(t_data *data, char **envp)
{
	int	count;

	if (!data || !envp)
		return ;
	if (!envp[0])
		return ;
	count = count_env_vars(envp);
	data->env->env = (char **)malloc(sizeof(char *) * (count + 1));
	if (!data->env->env)
		return ;
	allocate_env_copy(data, envp, count);
}

void	update_shlvl(t_data *data)
{
	int		i;
	int		shlvl;
	char	*new_shlvl;
	char	*value;

	if (!data || !data->env || !data->env->env)
		return ;
	i = 0;
	while (data->env->env[i])
	{
		if (ft_strncmp(data->env->env[i], "SHLVL=", 6) == 0)
		{
			value = data->env->env[i] + 6;
			shlvl = ft_atoi(value) + 1;
			new_shlvl = ft_itoa(shlvl);
			if (!new_shlvl)
				return ;
			free(data->env->env[i]);
			data->env->env[i] = ft_strjoin("SHLVL=", new_shlvl);
			free(new_shlvl);
			return ;
		}
		i++;
	}
}
