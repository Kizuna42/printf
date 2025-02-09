/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_s.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishino <kishino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 16:18:37 by kishino           #+#    #+#             */
/*   Updated: 2025/02/09 16:21:33 by kishino          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_flag_nonhyphen(t_tab *tab, char *s, long int diff)
{
	long int	count;

	count = 0;
	if (*s == *"" && tab->accuracy_width > 0)
	{
		if (tab->accuracy_width >= tab->field_width)
			diff += 0;
		else
			diff += 1;
	}
	if (tab->accuracy_width < 0)
		count = tab->accuracy_width * -1;
	while (count < diff)
	{
		if (tab->flag_index == 1)
			write(1, "0", 1);
		else
			write(1, " ", 1);
		tab->len++;
		count++;
	}
	if (tab->dot == 0 || (tab->dot == 1 && tab->accuracy_width < 0))
		tab->len += ft_putstr(s);
	else
		tab->len += ft_putnstr(s, tab->accuracy_width);
}

static void	ft_flag_hyphen(t_tab *tab, char *s, long int diff)
{
	long int	count;

	count = 0;
	if (tab->dot == 0 || tab->accuracy_width < 0)
	{
		if (tab->accuracy_width < 0)
			count = tab->field_width;
		tab->len += ft_putstr(s);
	}
	else
		tab->len += ft_putnstr(s, tab->accuracy_width);
	while (count < diff)
	{
		write(1, " ", 1);
		tab->len++;
		count++;
	}
}

void	ft_print_s(t_tab *tab)
{
	char		*s;
	long int	diff;

	s = (va_arg(tab->args, char *));
	if (s == NULL || (tab->accuracy_width < 0 && s == NULL))
	{
		ft_print_s_null(tab);
		return ;
	}
	tab->keta_count = ft_strlen(s);
	if (tab->field_width < 0)
	{
		tab->field_width *= -1;
		tab->flag_index = 2;
	}
	diff = tab->field_width - tab->keta_count;
	if (diff < tab->field_width - tab->accuracy_width && tab->dot != 0)
		diff = tab->field_width - tab->accuracy_width;
	if (tab->flag_index != 2)
		ft_flag_nonhyphen(tab, s, diff);
	else if (tab->flag_index == 2)
		ft_flag_hyphen(tab, s, diff);
}
