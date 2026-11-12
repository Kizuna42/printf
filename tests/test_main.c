/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 00:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2026/11/12 00:00:00 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_printf.h"

/*
** Plain text and %c. Formats without conversions get a dummy int argument,
** which both printf implementations evaluate and ignore.
*/
static void	test_text_and_c(t_stats *st)
{
	check_int(st, "hello, world", 0);
	check_int(st, "a\tb\nc", 0);
	check_int(st, "%%", 0);
	check_int(st, "100%% done", 0);
	check_int(st, "%c", 'a');
	case_three_chars(st);
	check_int(st, "[%c]", '\0');
	check_int(st, "%c", 200);
	check_int(st, "%c", 'A' + 256);
}

static void	test_s_d_i(t_stats *st)
{
	check_str(st, "%s", "");
	check_str(st, "%s", "hello");
	case_two_strs(st);
	check_int(st, "%d", 0);
	check_int(st, "%d", 42);
	check_int(st, "%d", -42);
	check_int(st, "%d", INT_MAX);
	check_int(st, "%d", INT_MIN);
	check_int(st, "%i", INT_MIN);
	case_two_ints(st);
}

static void	test_u_x(t_stats *st)
{
	check_uint(st, "%u", 0u);
	check_uint(st, "%u", UINT_MAX);
	check_int(st, "%u", -1);
	check_uint(st, "%x", 0u);
	check_uint(st, "%x", 255u);
	check_uint(st, "%X", 255u);
	check_uint(st, "%x", UINT_MAX);
	check_uint(st, "%X", 0xDEADBEEFu);
	check_int(st, "%x", -1);
}

/*
** NULL handling: "(null)" for %s, "0x0" for %p (as the macOS libc does).
*/
static void	test_p_and_null(t_stats *st, int *x)
{
	check_ptr(st, "%p", (void *)x);
	check_ptr(st, "%p", (void *)1);
	check_ptr(st, "%p", (void *)ULONG_MAX);
	case_mixed(st, (void *)x);
	expect_str(st, "(null)", "%s", NULL);
	expect_str(st, "[(null)]", "[%s]", NULL);
	expect_ptr(st, "0x0", "%p", NULL);
	expect_str(st, "", "%s", "");
	expect_empty_format(st);
}

int	main(void)
{
	t_stats	st;
	int		x;

	st.total = 0;
	st.fail = 0;
	x = 42;
	test_text_and_c(&st);
	test_s_d_i(&st);
	test_u_x(&st);
	test_p_and_null(&st, &x);
	printf("%d/%d passed\n", st.total - st.fail, st.total);
	return (st.fail != 0);
}
