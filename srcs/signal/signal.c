/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 02:38:46 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/20 02:32:41 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <readline/readline.h>

void			disable_signal(int signum);
void			report_signal(int signum);

static int	monitor_readline(void)
{
	extern t_status	g_status;

	if (g_status.signal % 256 == 0)
		return (0);
	else if (g_status.signal % 256 == SIGINT)
	{
		g_status.signal = 256;
		rl_replace_line("", 0);
		rl_done = true;
	}
	else if (g_status.signal % 256 == SIGQUIT)
	{
		write(STDERR_FILENO, "exit\n", 5);
	}
	return (0);
}

void	modify_signal(int signum)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = report_signal;
	if (sigaction(signum, &sa, NULL) < 0)
		fatal_error("sigaction");
}

void	setup_signal(void)
{
	extern int	_rl_echo_control_chars;

	_rl_echo_control_chars = false;
	rl_outstream = stderr;
	if (isatty(STDIN_FILENO))
	{
		rl_event_hook = monitor_readline;
	}
	disable_signal(SIGQUIT);
	modify_signal(SIGINT);
}
