/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 16:40:45 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/07/29 16:48:28 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strjoin_three(const char *s1, const char *s2, const char *s3)
{
	char	*result;
	size_t	len1;
	size_t	len2;
	size_t	len3;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len3 = ft_strlen(s3);
	result = malloc(len1 + len2 + len3 + 1);
	if (!result)
		return (NULL);
	ft_memcpy(result, s1, len1);
	ft_memcpy(result + len1, s2, len2);
	ft_memcpy(result + len1 + len2, s3, len3);
	result[len1 + len2 + len3] = '\0';
	return (result);
}

int	set_env_var(t_data *data, const char *key, const char *value)
{
	int		i;
	size_t	len;
	char	*new_var;

	if (!data || !key || !value)
		return (1);
	len = ft_strlen(key);
	new_var = ft_strjoin_three(key, "=", value);
	if (!new_var)
		return (1);
	i = 0;
	while (data->env->env[i])
	{
		if (ft_strncmp(data->env->env[i], key, len) == 0
			&& data->env->env[i][len] == '=')
		{
			free(data->env->env[i]);
			data->env->env[i] = new_var;
			return (0);
		}
		i++;
	}
	free(new_var);
	return (1);
}

int	update_pwd(t_data *data, char *oldpwd)
{
	char	cwd[BUFFER_SIZE];

	if (!getcwd(cwd, sizeof(cwd)))
		return (perror("cd"), 1);
	if (set_env_var(data, "OLDPWD", oldpwd) != 0)
		return (1);
	if (set_env_var(data, "PWD", cwd) != 0)
		return (1);
	return (0);
}

int	get_current_dir(char *cwd)
{
	if (!getcwd(cwd, BUFFER_SIZE))
	{
		perror("cd");
		return (1);
	}
	return (0);
}
