/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishino <kishino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 12:27:52 by kyuki             #+#    #+#             */
/*   Updated: 2025/02/09 16:38:21 by kishino          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_putnstr(const char *str, long int end)
{
	int	i;

	i = 0;
	if (str)
	{
		while (str[i] != '\0' && i < end)
		{
			write(1, str + i, 1);
			i++;
		}
	}
	return (i);
}
