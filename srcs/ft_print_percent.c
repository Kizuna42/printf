/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_percent.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishino <kishino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 16:18:11 by kishino           #+#    #+#             */
/*   Updated: 2025/02/09 16:40:40 by kishino          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_flag_hyphen(t_tab *tab)
{
	int	i;

	i = 0;
	write(1, &"%", 1);
	tab->len++;
	while (i < tab->field_width - 1)
	{
		write(1, " ", 1);
		tab->len++;
		i++;
	}
}

static void	ft_flag_nonhyphen(t_tab *tab)
{
	int	i;

	i = 0;
	while (i < tab->field_width - 1)
	{
		if (tab->flag_index != 1)
			write(1, " ", 1);
		else
			write(1, "0", 1);
		tab->len++;
		i++;
	}
	write(1, &"%", 1);
	tab->len++;
}

void	ft_print_percent(t_tab *tab)
{
	if (tab->flag_index != 2)
		ft_flag_nonhyphen(tab);
	else
		ft_flag_hyphen(tab);
}
