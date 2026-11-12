/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_capture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 00:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2026/11/12 00:00:00 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_printf.h"

/*
** Redirect stdout (fd 1) into a pipe so ft_printf's raw output can be read.
*/
void	cap_begin(t_case *c)
{
	fflush(stdout);
	if (pipe(c->pipe_fd) != 0)
	{
		perror("pipe");
		_exit(2);
	}
	c->saved_fd = dup(1);
	dup2(c->pipe_fd[1], 1);
	close(c->pipe_fd[1]);
}

void	cap_end(t_case *c)
{
	ssize_t	r;

	dup2(c->saved_fd, 1);
	close(c->saved_fd);
	c->got_len = 0;
	r = read(c->pipe_fd[0], c->got, BUF_SIZE - 1);
	while (r > 0)
	{
		c->got_len += (int)r;
		r = read(c->pipe_fd[0], c->got + c->got_len,
				BUF_SIZE - 1 - c->got_len);
	}
	close(c->pipe_fd[0]);
	c->got[c->got_len] = '\0';
}

void	report(t_stats *st, t_case *c)
{
	st->total++;
	if (c->got_ret == c->exp_ret && c->got_len == c->exp_ret
		&& memcmp(c->got, c->exp, (size_t)c->exp_ret) == 0)
		return ;
	st->fail++;
	printf("FAIL case %d: \"%s\"\n", st->total, c->label);
	printf("  expected [%.*s] ret=%d\n", c->exp_ret, c->exp, c->exp_ret);
	printf("  got      [%.*s] ret=%d\n", c->got_len, c->got, c->got_ret);
}
