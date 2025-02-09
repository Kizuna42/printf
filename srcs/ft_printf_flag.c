/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_flag.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kishino <kishino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 16:22:38 by kishino           #+#    #+#             */
/*   Updated: 2025/02/09 16:30:59 by kishino          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_printf_flag(t_tab *tab, int *i)
{
	if (tab->copy[*i] == '0' && tab->copy[*i + 1] == '-')
		*i += 1;
	tab->flag_index = ft_strchr(tab->flag_kind, tab->copy[*i]);
	if (tab->flag_index >= 0)
		*i += 1;
	if (tab->copy[*i] == '*')
	{
		*i += 1;
		tab->asterisk += 1;
	}
	if ('0' <= tab->copy[*i] && tab->copy[*i] <= '9')
		tab->field_width = ft_atoi(&tab->copy[*i], &*i);
	if (tab->copy[*i] == '.')
	{
		*i += 1;
		tab->dot += 1;
		tab->accuracy_width = ft_atoi(&tab->copy[*i], &*i);
	}
	if (tab->copy[*i] == '*')
	{
		*i += 1;
		tab->asterisk += 1;
	}
}
