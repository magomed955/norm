/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 18:31:51 by mubersan          #+#    #+#             */
/*   Updated: 2025/07/29 17:36:28 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	int		i;
	int		len;
	char	*strcpy;

	i = 0;
	len = 0;
	if (!s)
		return (NULL);
	while (s[len])
	{
		len++;
	}
	strcpy = (char *)malloc(len + 1);
	if (!strcpy)
		return (NULL);
	while (s[i] != '\0')
	{
		strcpy[i] = s[i];
		i++;
	}
	strcpy[i] = '\0';
	return (strcpy);
}
