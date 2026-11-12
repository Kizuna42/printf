/*
** Compare ft_printf with the libc printf family for the mandatory
** conversions (c s p d i u x X %). Output bytes and return values must match.
** Build and run with: make test
*/
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ft_printf.h"

#define BUF_SIZE 4096

static int	g_fail = 0;
static int	g_total = 0;
static int	g_saved_fd = -1;
static int	g_pipe[2];

static void	begin_capture(void)
{
	fflush(stdout);
	if (pipe(g_pipe) != 0)
	{
		perror("pipe");
		_exit(2);
	}
	g_saved_fd = dup(1);
	dup2(g_pipe[1], 1);
	close(g_pipe[1]);
}

static int	end_capture(char *out)
{
	ssize_t	n;
	ssize_t	r;

	dup2(g_saved_fd, 1);
	close(g_saved_fd);
	n = 0;
	while ((r = read(g_pipe[0], out + n, BUF_SIZE - 1 - n)) > 0)
		n += r;
	close(g_pipe[0]);
	out[n] = '\0';
	return ((int)n);
}

static void	report(const char *src, int line, const char *got, int got_len,
		int got_ret, const char *exp, int exp_ret)
{
	g_total++;
	if (got_ret == exp_ret && got_len == exp_ret
		&& memcmp(got, exp, (size_t)exp_ret) == 0)
		return ;
	g_fail++;
	printf("FAIL line %d: %s\n  expected [%.*s] ret=%d\n  got      [%.*s] ret=%d\n",
		line, src, exp_ret, exp, exp_ret, got_len, got, got_ret);
}

#define CHECK(...) do { \
	char	got_[BUF_SIZE]; \
	char	exp_[BUF_SIZE]; \
	int		got_ret_; \
	int		got_len_; \
	int		exp_ret_; \
	exp_ret_ = snprintf(exp_, BUF_SIZE, __VA_ARGS__); \
	begin_capture(); \
	got_ret_ = ft_printf(__VA_ARGS__); \
	got_len_ = end_capture(got_); \
	report(#__VA_ARGS__, __LINE__, got_, got_len_, got_ret_, exp_, exp_ret_); \
} while (0)

/* Compare against a fixed expected string (for implementation-defined cases). */
#define CHECK_EXPECT(expected, ...) do { \
	char	got_[BUF_SIZE]; \
	int		got_ret_; \
	int		got_len_; \
	begin_capture(); \
	got_ret_ = ft_printf(__VA_ARGS__); \
	got_len_ = end_capture(got_); \
	report(#__VA_ARGS__, __LINE__, got_, got_len_, got_ret_, expected, \
		(int)strlen(expected)); \
} while (0)

int	main(void)
{
	int		x;
	char	*null_str;

	x = 42;
	null_str = NULL;
	/* plain text */
	CHECK("hello, world");
	CHECK("a\tb\nc");
	CHECK("%%");
	CHECK("100%% done");
	/* %c */
	CHECK("%c", 'a');
	CHECK("%c%c%c", 'x', 'y', 'z');
	CHECK("[%c]", '\0');
	CHECK("%c", 200);
	CHECK("%c", 'A' + 256);
	/* %s */
	CHECK("%s", "");
	CHECK("%s", "hello");
	CHECK("%s and %s", "foo", "bar");
	/* %d %i */
	CHECK("%d", 0);
	CHECK("%d", 42);
	CHECK("%d", -42);
	CHECK("%d", INT_MAX);
	CHECK("%d", INT_MIN);
	CHECK("%i", INT_MIN);
	CHECK("%i %d", -1, 1);
	/* %u */
	CHECK("%u", 0u);
	CHECK("%u", UINT_MAX);
	CHECK("%u", -1);
	/* %x %X */
	CHECK("%x", 0u);
	CHECK("%x", 255u);
	CHECK("%X", 255u);
	CHECK("%x", UINT_MAX);
	CHECK("%X", 0xDEADBEEFu);
	CHECK("%x", -1);
	/* %p */
	CHECK("%p", (void *)&x);
	CHECK("%p", (void *)1);
	CHECK("%p", (void *)ULONG_MAX);
	/* mixed */
	CHECK("%c|%s|%d|%i|%u|%x|%X|%%|%p", 'q', "str", -7, 7, 7u, 0xabu,
		0xabu, (void *)&x);
	/* NULL handling: "(null)" for %s, "0x0" for %p (as the macOS libc does) */
	CHECK_EXPECT("(null)", "%s", null_str);
	CHECK_EXPECT("[(null)]", "[%s]", null_str);
	CHECK_EXPECT("0x0", "%p", (void *)0);
	/* empty format writes nothing and returns 0; NULL format returns -1 */
	CHECK_EXPECT("", "%s", "");
	{
		char	got_[BUF_SIZE];
		int		ret_;

		begin_capture();
		ret_ = ft_printf("");
		report("\"\"", __LINE__, got_, end_capture(got_), ret_, "", 0);
	}
	printf("%d/%d passed\n", g_total - g_fail, g_total);
	return (g_fail != 0);
}
