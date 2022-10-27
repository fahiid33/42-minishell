/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 03:05:24 by fstitou           #+#    #+#             */
/*   Updated: 2022/10/27 03:22:36 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	permission_denied(char *file)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": Permission denied\n", 2);
	g_vars.exit_status = 1;
	g_vars.g_err = 1;
	return (-1);
}

int	check_file(t_redir *redir, int mode)
{
	if (mode == 1)
	{
		redir->fdout = open(redir->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (access(redir->file, W_OK) != 0)
			return (permission_denied(redir->file));
		return (redir->fdout);
	}
	else if (mode == 2)
	{
		redir->fdout = open(redir->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (access(redir->file, W_OK) != 0)
			return (permission_denied(redir->file));
		return (redir->fdout);
	}
	else if (mode == 3)
	{
		redir->fdin = open(redir->file, O_RDONLY);
		if (access(redir->file, R_OK) != 0)
			return (permission_denied(redir->file));
		return (redir->fdin);
	}
	return (0);
}

int	read_error(char *file, int exec)
{
	file_error(file, 0);
	if (exec)
	{
		g_vars.g_err = 1;
		return (-1);
	}
	return (0);
}

int	open_files(t_redir *redir, int mode)
{
	if (opendir(redir->file))
	{
		ft_putstr_fd("minshell: : Is a directory\n", 2);
		g_vars.g_err = 1;
		g_vars.exit_status = 1;
		return (-1);
	}
	if (mode == 1)
		return (check_file(redir, mode));
	else if (mode == 2)
		return (check_file(redir, mode));
	else if (mode == 3)
		return (check_file(redir, mode));
	return (0);
}
