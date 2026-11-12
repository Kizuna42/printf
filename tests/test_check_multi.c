/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_check_multi.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 00:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2026/11/12 00:00:00 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_printf.h"

void	case_three_chars(t_stats *st)
{
	t_case	c;

	c.label = "%c%c%c";
	c.exp_ret = snprintf(c.exp, BUF_SIZE, "%c%c%c", 'x', 'y', 'z');
	cap_begin(&c);
	c.got_ret = ft_printf("%c%c%c", 'x', 'y', 'z');
	cap_end(&c);
	report(st, &c);
}

void	case_two_strs(t_stats *st)
{
	t_case	c;

	c.label = "%s and %s";
	c.exp_ret = snprintf(c.exp, BUF_SIZE, "%s and %s", "foo", "bar");
	cap_begin(&c);
	c.got_ret = ft_printf("%s and %s", "foo", "bar");
	cap_end(&c);
	report(st, &c);
}

void	case_two_ints(t_stats *st)
{
	t_case	c;

	c.label = "%i %d";
	c.exp_ret = snprintf(c.exp, BUF_SIZE, "%i %d", -1, 1);
	cap_begin(&c);
	c.got_ret = ft_printf("%i %d", -1, 1);
	cap_end(&c);
	report(st, &c);
}

void	case_mixed(t_stats *st, void *p)
{
	t_case	c;

	c.label = "%c|%s|%d|%i|%u|%x|%X|%%|%p";
	c.exp_ret = snprintf(c.exp, BUF_SIZE, "%c|%s|%d|%i|%u|%x|%X|%%|%p",
			'q', "str", -7, 7, 7u, 0xabu, 0xabu, p);
	cap_begin(&c);
	c.got_ret = ft_printf("%c|%s|%d|%i|%u|%x|%X|%%|%p",
			'q', "str", -7, 7, 7u, 0xabu, 0xabu, p);
	cap_end(&c);
	report(st, &c);
}
