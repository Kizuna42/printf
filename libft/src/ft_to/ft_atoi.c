/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishino <kishino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 15:01:34 by kishino           #+#    #+#             */
/*   Updated: 2025/02/09 16:03:23 by kishino          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_atoi(const char *str, int *len)
{
	unsigned long long		atoi;
	int						negative;

	atoi = 0;
	negative = 1;
	while (*str == '\t' || *str == '\v' || *str == '\n'
		|| *str == '\r' || *str == '\f' || *str == ' ')
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			negative *= -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		atoi = atoi * 10 + *str - 48;
		str++;
		(*len)++;
		if (atoi > 9223372036854775807 && negative == -1)
			return (0);
		if (atoi > 9223372036854775806 && negative == 1)
			return (-1);
	}
	return ((int)atoi * negative);
}

// #include <stdio.h>

// int main(void)
// {
// 	int i;
// 	char *s;

// 	s = " 		-9885";
// 	i = ft_atoi(s);
// 	printf("i = %d\n",i);
// }

/*******************  Output should be similar to:  ***************

i = -9885
*/
