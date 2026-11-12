/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 00:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2026/11/12 00:00:00 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_printf.h"

/*
** Run one conversion through both snprintf (expected) and ft_printf (got).
*/
void	check_int(t_stats *st, const char *fmt, int v)
{
	t_case	c;

	c.label = fmt;
	c.exp_ret = snprintf(c.exp, BUF_SIZE, fmt, v);
	cap_begin(&c);
	c.got_ret = ft_printf(fmt, v);
	cap_end(&c);
	report(st, &c);
}

void	check_uint(t_stats *st, const char *fmt, unsigned int v)
{
	t_case	c;

	c.label = fmt;
	c.exp_ret = snprintf(c.exp, BUF_SIZE, fmt, v);
	cap_begin(&c);
	c.got_ret = ft_printf(fmt, v);
	cap_end(&c);
	report(st, &c);
}

void	check_str(t_stats *st, const char *fmt, const char *v)
{
	t_case	c;

	c.label = fmt;
	c.exp_ret = snprintf(c.exp, BUF_SIZE, fmt, v);
	cap_begin(&c);
	c.got_ret = ft_printf(fmt, v);
	cap_end(&c);
	report(st, &c);
}

void	check_ptr(t_stats *st, const char *fmt, void *v)
{
	t_case	c;

	c.label = fmt;
	c.exp_ret = snprintf(c.exp, BUF_SIZE, fmt, v);
	cap_begin(&c);
	c.got_ret = ft_printf(fmt, v);
	cap_end(&c);
	report(st, &c);
}
