/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 18:44:31 by yutakagi          #+#    #+#             */
/*   Updated: 2024/03/20 17:45:09 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	reset_signal(int signum)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = SIG_DFL;
	if (sigaction(signum, &sa, NULL) < 0)
		fatal_error("sigaction");
}

void	reset_signals(void)
{
	reset_signal(SIGINT);
	reset_signal(SIGQUIT);
}

void	disable_signal(int signum)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = SIG_IGN;
	if (sigaction(signum, &sa, NULL) < 0)
		fatal_error("sigaction");
}

void	report_signal(int signum)
{
	extern int	g_signal;

	g_signal = signum;
}
