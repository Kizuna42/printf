/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_expect.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 00:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2026/11/12 00:00:00 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_printf.h"

/*
** Compare against a fixed expected string (for implementation-defined cases).
*/
void	expect_str(t_stats *st, const char *exp, const char *fmt,
			const char *v)
{
	t_case	c;

	c.label = fmt;
	c.exp_ret = (int)strlen(exp);
	memcpy(c.exp, exp, (size_t)c.exp_ret + 1);
	cap_begin(&c);
	c.got_ret = ft_printf(fmt, v);
	cap_end(&c);
	report(st, &c);
}

void	expect_ptr(t_stats *st, const char *exp, const char *fmt, void *v)
{
	t_case	c;

	c.label = fmt;
	c.exp_ret = (int)strlen(exp);
	memcpy(c.exp, exp, (size_t)c.exp_ret + 1);
	cap_begin(&c);
	c.got_ret = ft_printf(fmt, v);
	cap_end(&c);
	report(st, &c);
}

/*
** An empty format writes nothing and returns 0.
*/
void	expect_empty_format(t_stats *st)
{
	t_case	c;

	c.label = "";
	c.exp[0] = '\0';
	c.exp_ret = 0;
	cap_begin(&c);
	c.got_ret = ft_printf("");
	cap_end(&c);
	report(st, &c);
}
