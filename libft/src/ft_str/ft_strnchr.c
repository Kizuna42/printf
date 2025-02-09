/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishino <kishino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 12:28:04 by kyuki             #+#    #+#             */
/*   Updated: 2025/02/09 17:54:28 by kishino          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_strnchr(const char *s, int c, int start, int end)
{
	while (s[start] != (const char)end)
	{
		if (((unsigned char *)s)[start] == (unsigned char)c)
			return (start);
		start++;
	}
	return (-1);
}
