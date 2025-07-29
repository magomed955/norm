/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 16:55:21 by mmutsulk          #+#    #+#             */
/*   Updated: 2025/07/29 16:59:04 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!ft_isdigit(str[i]))
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	adder(uint64_t a, uint64_t b, uint64_t *final)
{
	uint64_t	carry;
	uint64_t	limit;

	carry = 0;
	limit = 1;
	limit <<= 63;
	while (b != 0)
	{
		carry = a & b;
		a = a ^ b;
		if (a > limit)
			return (0);
		b = carry << 1;
	}
	*final = a;
	return (1);
}

int	ft_add_and_multiply(int digit, uint64_t *final)
{
	int			j;
	uint64_t	temp;
	uint64_t	tmp_final;

	j = 0;
	temp = 0;
	tmp_final = *final;
	while (j < 10)
	{
		if (!adder(temp, tmp_final, &temp))
			return (0);
		j++;
	}
	if (!adder(temp, digit, final))
		return (0);
	return (1);
}
