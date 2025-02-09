/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_c.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishino <kishino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 16:13:28 by kishino           #+#    #+#             */
/*   Updated: 2025/02/09 16:40:25 by kishino          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_flag_nonhyphen(t_tab *tab, char *s)
{
	int	i;

	i = 0;
	while (i < tab->field_width - 1)
	{
		if (tab->flag_index == 1)
			write(1, "0", 1);
		else
			write(1, " ", 1);
		tab->len++;
		i++;
	}
	write(1, &s, 1);
	tab->len++;
}

static void	ft_flag_hyphen(t_tab *tab, char *s)
{
	int	i;

	i = 0;
	write(1, &s, 1);
	tab->len++;
	while (i < tab->field_width - 1)
	{
		write(1, " ", 1);
		tab->len++;
		i++;
	}
}

void	ft_print_c(t_tab *tab)
{
	char	*s;

	s = (va_arg(tab->args, char *));
	if (tab->flag_index != 2)
		ft_flag_nonhyphen(tab, s);
	else
		ft_flag_hyphen(tab, s);
}
