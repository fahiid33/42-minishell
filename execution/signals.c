/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubare <amoubare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 03:00:20 by fstitou           #+#    #+#             */
/*   Updated: 2022/10/28 04:36:55 by amoubare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sig_helper(void)
{
	if (!g_vars.line)
	{
		ft_putchar_fd('\n', 0);
		rl_on_new_line();
		// rl_replace_line("", 0);
		rl_redisplay();
		g_vars.exit_status = 1;
	}
	else if (g_vars.pid != 0)
	{
		ft_putchar_fd('\n', 0);
	}
}

void	main_sig(int sig)
{
	if (sig == SIGINT)
	{
		if (g_vars.exit_sig == 27)
		{
			g_vars.exit_sig = -27;
			ft_putchar_fd('\n', 0);
			close(rl_instream->_file);
			g_vars.exit_status = 1;
		}
		else
			sig_helper();
	}
	else if (sig == SIGQUIT)
	{
		if (!g_vars.line)
		{
			ft_putchar_fd('\r', STDERR_FILENO);
			rl_on_new_line();
			rl_redisplay();
		}
	}
}

void	sig_handler(int sig)
{
	if (!kill(g_vars.pid, sig))
	{
		g_vars.pid = 0;
		if (sig == SIGQUIT)
		{
			ft_putstr_fd("Quit: 3\n", 1);
			g_vars.exit_status = 131;
		}
		else if (sig == SIGINT)
		{
			ft_putstr_fd("\n", 1);
			g_vars.exit_status = 130;
		}
	}
	else if (g_vars.pid != 0)
		main_sig(sig);
}

void	ctrls(int sig)
{
	if (g_vars.pid != 0)
		sig_handler(sig);
	else
		main_sig(sig);
}

void	c_signal(void)
{
	signal(SIGINT, ctrls);
	signal(SIGQUIT, ctrls);
}
