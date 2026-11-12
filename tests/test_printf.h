/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_printf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 00:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2026/11/12 00:00:00 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_PRINTF_H
# define TEST_PRINTF_H

/*
** Compare ft_printf with the libc printf family for the mandatory
** conversions (c s p d i u x X %). Output bytes and return values must match.
** Build and run with: make test
*/

# include <limits.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include "ft_printf.h"

# define BUF_SIZE 4096

typedef struct s_stats
{
	int	total;
	int	fail;
}	t_stats;

typedef struct s_case
{
	const char	*label;
	char		exp[BUF_SIZE];
	int			exp_ret;
	char		got[BUF_SIZE];
	int			got_len;
	int			got_ret;
	int			pipe_fd[2];
	int			saved_fd;
}	t_case;

void	cap_begin(t_case *c);
void	cap_end(t_case *c);
void	report(t_stats *st, t_case *c);

void	check_int(t_stats *st, const char *fmt, int v);
void	check_uint(t_stats *st, const char *fmt, unsigned int v);
void	check_str(t_stats *st, const char *fmt, const char *v);
void	check_ptr(t_stats *st, const char *fmt, void *v);

void	case_three_chars(t_stats *st);
void	case_two_strs(t_stats *st);
void	case_two_ints(t_stats *st);
void	case_mixed(t_stats *st, void *p);

void	expect_str(t_stats *st, const char *exp, const char *fmt,
			const char *v);
void	expect_ptr(t_stats *st, const char *exp, const char *fmt, void *v);
void	expect_empty_format(t_stats *st);

#endif
