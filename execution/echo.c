/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 02:30:26 by fahd              #+#    #+#             */
/*   Updated: 2022/10/10 03:16:40 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	echo_n_print(t_parse *cmd, int i)
{
	while (cmd->argv[i])
	{
		ft_putstr_fd(cmd->argv[i], 1);
		i++;
		if (cmd->argv[i])
			write(1, " ", 1);
	}
}

void	echo_e(t_parse *cmd)
{
	int	i;

	i = 0;
	while (cmd->argv[i])
	{
		ft_putstr_fd(cmd->argv[i], 1);
		i++;
		if (cmd->argv[i])
			write(STDOUT_FILENO, " ", 1);
	}
	write(STDOUT_FILENO, "\n", 1);
}

void	echo_n(t_parse *cmd)
{
	int	i;

	i = 0;
	while (cmd->argv && cmd->argv[i])
	{
		if (check_minus(cmd->argv[i], 'n') == 0)
			break ;
		i++;
	}
	if (i == 0)
		echo_e(cmd);
	else if (i != 0 && cmd->argv && cmd->argv[i])
	{
		echo_n_print(cmd, i);
		return ;
	}
	return ;
}

int	echo(t_parse *cmd)
{
	if (!cmd->argv[0])
	{
		write(STDOUT_FILENO, "\n", 1);
		g_vars.exit_status = 0;
		return (g_vars.exit_status);
	}
	else
	{		
		if (strncmp(cmd->argv[0], "-n", 2) == 0)
			echo_n(cmd);
		else
			echo_e(cmd);
	}
	g_vars.exit_status = 0;
	return (g_vars.exit_status);
}
