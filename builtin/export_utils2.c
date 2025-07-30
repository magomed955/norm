/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mubersan <mubersan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 17:24:35 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/07/30 19:28:21 by mubersan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_export_var(char *var)
{
	char	*eq;

	ft_putstr_fd("declare -x ", 1);
	eq = ft_strchr(var, '=');
	if (eq)
	{
		*eq = '\0';
		ft_putstr_fd(var, 1);
		ft_putstr_fd("=\"", 1);
		if (*(eq + 1))
			ft_putstr_fd(eq + 1, 1);
		ft_putstr_fd("\"\n", 1);
		*eq = '=';
	}
	else
	{
		ft_putstr_fd(var, 1);
		write(1, "\n", 1);
	}
}

void	export_display(t_data *data)
{
	char	**combined;
	char	**sorted;
	int		env_len;
	int		exp_len;
	int		i;

	env_len = ft_array_len(data->env->env);
	exp_len = ft_array_len(data->env->export_only);
	i = 0;
	combined = malloc(sizeof(char *) * (env_len + exp_len + 1));
	while (i < env_len)
	{
		combined[i] = data->env->env[i];
		i++;
	}
	for (int j = 0; j < exp_len; j++)
		combined[i++] = data->env->export_only[j];
	combined[i] = NULL;
	sorted = ft_strdup_array(combined);
	ft_sort_ascii(sorted);
	i = 0;
	while (sorted[i])
		print_export_var(sorted[i++]);
	free_array(sorted);
	free(combined);
}

int	ft_array_len(char **array)
{
	int	i = 0;

	if (!array)
		return (0);
	while (array[i])
		i++;
	return (i);
}

void	remove_export_only_entry(t_data *data, const char *var_name)
{
	int		len = ft_array_len(data->env->export_only);
	char	**new_export = malloc(sizeof(char *) * (len + 1));
	int		i = 0, j = 0;

	if (!new_export)
		return ;
	if (len != 0)
	{
		while (data->env->export_only[i])
		{
			if (ft_strncmp(data->env->export_only[i], var_name,
					ft_strlen(var_name)) == 0
				&& (data->env->export_only[i][ft_strlen(var_name)] == '\0'
					|| data->env->export_only[i][ft_strlen(var_name)] == '='))
				free(data->env->export_only[i]);
			else
				new_export[j++] = data->env->export_only[i];
			i++;
		}
	}
	new_export[j] = NULL;
	free(data->env->export_only);
	data->env->export_only = new_export;
}

int	is_var_in_array(char **array, const char *var_name)
{
	int		i;
	size_t	len = ft_strlen(var_name);

	i = 0;
	while (array && array[i])
	{
		if (!ft_strncmp(array[i], var_name, len) &&
			(array[i][len] == '=' || array[i][len] == '\0'))
			return (1);
		i++;
	}
	return (0);
}
