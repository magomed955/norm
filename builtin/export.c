/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 14:31:02 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/07/29 21:50:03 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	update_existing_var(t_data *data, char *arg, char *var_name, size_t len)
{
	int	i;

	i = 0;
	while (data->env->env[i])
	{
		if (!ft_strncmp(data->env->env[i], var_name, len)
			&& (data->env->env[i][len] == '=' || !data->env->env[i][len]))
		{
			free(data->env->env[i]);
			data->env->env[i] = ft_strdup(arg);
			return (1);
		}
		i++;
	}
	return (0);
}

void	add_new_var(t_data *data, char *arg, char *equal_pos)
{
	if (equal_pos)
		data->env->env = ft_add_to_array(data->env->env, arg);
	else
		data->env->export_only = ft_add_to_array(data->env->export_only, arg);
}

void	export_add_or_update(t_data *data, char *arg)
{
	char	*var_name;
	size_t	len;
	char	*equal_pos;

	equal_pos = ft_strchr(arg, '=');
	len = equal_pos ? (size_t)(equal_pos - arg) : ft_strlen(arg);
	var_name = ft_substr(arg, 0, len);
	if (equal_pos)
	{
		if (!update_existing_var(data, arg, var_name, len))
			data->env->env = ft_add_to_array(data->env->env, arg);
		remove_export_only_entry(data, var_name);
	}
	else if (!is_var_in_array(data->env->env, var_name)
		&& !is_var_in_array(data->env->export_only, var_name))
	{
		data->env->export_only = ft_add_to_array(data->env->export_only, arg);
	}
	free(var_name);
}

int	is_valid_export_identifier(const char *s)
{
	int	i;

	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	ft_export(t_data *data, char **args)
{
	int	i;
	int	err;

	err = 0;
	if (!args[1])
		export_display(data);
	else
	{
		i = 1;
		while (args[i])
		{
			if (is_valid_export_identifier(args[i]))
				export_add_or_update(data, args[i]);
			else
			{
				ft_puterror("minishell: export: `", args[i],
					"': not a valid identifier");
				err = 1;
			}
			i++;
		}
	}
	*data->exit->exit = err;
	return (0);
}
