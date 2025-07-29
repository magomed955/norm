/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 16:54:27 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/07/29 17:16:26 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_parse_loop(char *nptr, int *i, uint64_t *nb)
{
	while (nptr[*i] >= '0' && nptr[*i] <= '9')
	{
		if (!ft_add_and_multiply(nptr[*i] - '0', nb))
			return (0);
		(*i)++;
	}
	return (1);
}

void	ft_parse_sign(char *nptr, int *i, int *sign)
{
	if (nptr[*i] == '-' || nptr[*i] == '+')
	{
		if (nptr[*i] == '-')
			*sign *= -1;
		(*i)++;
	}
}

int	ft_atoi_no_overflow(char *nptr, int *final)
{
	int			i;
	int			sign;
	uint64_t	nb;

	i = 0;
	nb = 0;
	sign = 1;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	ft_parse_sign(nptr, &i, &sign);
	if (!ft_parse_loop(nptr, &i, &nb))
		return (0);
	if (sign == 1 && nb == ((uint64_t)1 << 63))
		return (0);
	*final = (int64_t)nb;
	*final %= 256;
	if (sign == 1 || !*final)
		*final = nb;
	else
		*final = 256 - *final;
	return (1);
}

int	handle_exit_error(char **args)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(args[0], STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	return (2);
}

void	ft_exit(char **args, t_data *data)
{
	int	exit_code;

	ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (!args || !args[0])
		exit_code = *data->exit->exit;
	else if (!is_numeric(args[0]))
		exit_code = handle_exit_error(args);
	else if (args[1])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		*data->exit->exit = 1;
		return ;
	}
	else
	{
		if (!ft_atoi_no_overflow(args[0], &exit_code))
			exit_code = handle_exit_error(args);
	}
	*data->exit->exit = exit_code;
	data->should_exit = 1;
}
