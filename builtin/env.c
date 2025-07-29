/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 09:58:58 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/07/29 16:33:39 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_env(t_data *data)
{
	int	i;

	if (!data || !data->env || !data->env->env)
	{
		ft_putstr_fd("minishell: env: No environment variables found\n", 2);
		return ;
	}
	i = 0;
	while (data->env->env[i])
	{
		ft_putendl_fd(data->env->env[i], 1);
		i++;
	}
}
