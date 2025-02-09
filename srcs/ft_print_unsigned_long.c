/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_unsigned_long.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishino <kishino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 16:22:23 by kishino           #+#    #+#             */
/*   Updated: 2025/02/09 18:20:13 by kishino          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_putnbr_ulong(t_tab *tab, unsigned long nbr, char mode)
{
	int	len;

	len = 0;
	if (mode != 'l')
	{
		ft_put_zero(tab);
		if (nbr == 0 && (tab->field_width - tab->accuracy_width) > 0
			&& tab->accuracy_width == 0 && tab->dot > 0)
			return (1);
		else
			len += ft_putnbr_write_len(
					nbr, tab->base, mode, "0123456789abcdef");
	}
	else
		len += ft_putnbr_write_len(nbr, tab->base, mode, "0123456789abcdef");
	return (len);
}

static void	ft_flag_hyphen(t_tab *tab, unsigned long p, int diff)
{
	int	count;

	write(1, "0x", 2);
	tab->len += 2;
	tab->len += ft_putnbr_ulong(tab, p, 'w');
	count = 0;
	if (diff > 0)
	{
		while (count < tab->field_width
			- (tab->keta_count + tab->acu_keta_diff + tab->negative_other))
		{
			write(1, " ", 1);
			tab->len++;
			count++;
		}
	}
}

static void	ft_flag_nonhyphen(t_tab *tab, unsigned long p, int diff)
{
	int	count;

	count = 0;
	if (diff > 0)
		ft_diff_one_or_more(tab, count);
	else
		write(1, "0x", 2);
	tab->len += 2;
	tab->len += ft_putnbr_ulong(tab, p, 'w');
}

void	ft_print_unsigned_long(t_tab *tab, int base)
{
	unsigned long	p;
	int				diff;

	p = (va_arg(tab->args, unsigned long));
	tab->base = base;
	if (p == 0)
	{
		write(1, "(nil)", 5);
		tab->len += 5;
		return ;
	}
	tab->negative_other = 2;
	tab->keta_count += ft_putnbr_ulong(tab, p, 'l');
	diff = ft_set_diff(tab);
	if ((tab->dot > 0 && (p == 0 || !p) && tab->accuracy_width == 0
			&& (tab->field_width >= 0 && tab->field_width <= 2)))
	{
		write(1, "0x", 2);
		tab->len += 2;
		return ;
	}
	if (tab->flag_index != 2)
		ft_flag_nonhyphen(tab, p, diff);
	else if (tab->flag_index == 2)
		ft_flag_hyphen(tab, p, diff);
}
