/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_flag.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 16:22:38 by kishino           #+#    #+#             */
/*   Updated: 2025/03/04 18:38:00 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

static void	ft_process_flag_priority(t_tab *tab, int *i)
{
	int	current_flag;

	while (ft_strchr(tab->flag_kind, tab->copy[*i]) >= 0)
	{
		current_flag = ft_strchr(tab->flag_kind, tab->copy[*i]);
		if (current_flag == 2)
		{
			tab->flag_index = 2;
			*i += 1;
			continue ;
		}
		else if (current_flag == 3 && tab->flag_index != 2)
		{
			tab->flag_index = 3;
		}
		else if (current_flag == 4 && tab->flag_index != 2
			&& tab->flag_index != 3)
		{
			tab->flag_index = 4;
		}
		*i += 1;
	}
}

static void	ft_process_flag(t_tab *tab, int *i)
{
	tab->flag_index = ft_strchr(tab->flag_kind, tab->copy[*i]);
	if (tab->flag_index >= 0)
		*i += 1;
	if (tab->flag_index == 2)
		return ;
	ft_process_flag_priority(tab, i);
}

static void	ft_process_width_precision(t_tab *tab, int *i)
{
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

void	ft_printf_flag(t_tab *tab, int *i)
{
	while (tab->copy[*i] == '-' || tab->copy[*i] == '0')
	{
		if (tab->copy[*i] == '-')
			tab->flag_index = 2;
		*i += 1;
	}
	if (ft_strchr(tab->flag_kind, tab->copy[*i]) >= 0)
		ft_process_flag(tab, i);
	ft_process_width_precision(tab, i);
}
