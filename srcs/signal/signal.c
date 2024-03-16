/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 02:38:46 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/16 21:43:44 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <readline/readline.h>

extern t_status status;
void disable_signal(int signum);
void report_signal(int signum);

static int	monitor_readline(void)
{
	if (status.signal == 0)
		return (0);
	else if (status.signal == SIGINT)
	{
		status.signal =  0;
		status.is_interrupted = true;
		rl_replace_line("", 0);
		rl_done = true;
	}
	else if (status.signal == SIGQUIT)
	{
		write(STDERR_FILENO, "exit\n", 5);
	}
	return (0);
}

void modify_signal(int signum)
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
	extern int _rl_echo_control_chars;

	_rl_echo_control_chars = false;
	rl_outstream = stderr;
	if (isatty(STDIN_FILENO))
	{
		rl_event_hook = monitor_readline;
	}
	disable_signal(SIGQUIT);
	modify_signal(SIGINT);
}
