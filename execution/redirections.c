/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/30 18:45:13 by fahd              #+#    #+#             */
/*   Updated: 2022/10/24 05:53:38 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	dup_pipes(t_parse *cmd, int in, int i, int *fd)
{
	if (cmd->next->cmd != NULL || cmd->next->redir != NULL)
	{
		dup2(fd[1], 1);
		close(fd[0]);
		close(fd[1]);
	}
	if (i != 0)
	{
		dup2(in, 0);
		close(fd[1]);
		close(fd[0]);
	}
}

void	pipe_redir(t_parse *cmd, int in, int index, int *fd)
{
	dup_pipes(cmd, in, index, fd);
	open_redir(cmd, 0);
}

int	open_files(t_redir *redir, int mode)
{
	if (opendir(redir->file))
	{
		ft_putstr_fd("minshell: : Is a directory\n", 2);
		return (0);
	}
	if(mode == 1)
	{
		redir->fdout = open(redir->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		return (redir->fdout);
	}
	else if (mode == 2)
	{
		redir->fdout = open(redir->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
		return (redir->fdout);
	}
	return (0);
}

int	append_trunc(t_redir *redir)
{
	int	fout;

	fout = 1;
	if (redir->file[0] == '*' || redir->file[0] == '\0')
		return (ambg_redir(redir->file));
	if (redir->e_type == GREAT)
	{
		if (open_files(redir, 1) == 0)
			return (2);
		fout = redir->fdout;
	}
	else
	{
		if (open_files(redir, 2) == 0)
			return (2);
		fout = redir->fdout;
	}
	return (fout);
}

int	open_read(t_redir *redir, t_parse *cmd, int exec)
{
	int	fin;

	fin = 0;
	if (redir->file[0] == '*' || redir->file[0] == '\0')
		return (ambg_redir(redir->file));
	if (redir->e_type == LESS)
	{	
		if (access(redir->file, F_OK) == -1)
		{
			file_error(redir->file, 0);
			if (exec)
				return (-1);
			exit(g_vars.exit_status);
		}
		else
		{
			if (opendir(redir->file))
				ft_putstr_fd("minshell: : Is a directory\n", 2);
			redir->fdin = open(redir->file, O_RDONLY);
			fin = redir->fdin;
		}
	}
	else
		if (cmd->cmd)
			dup2(redir->fdin, 0);
	return (fin);
}

void	open_redir(t_parse *head, int exe)
{
	t_redir	*tmp;
	int		fout;
	int		fin;

	fin = 0;
	fout = 1;
	tmp = head->redir;
	while (tmp != NULL)
	{
		if (tmp->e_type == GREAT || tmp->e_type == GREATANDGREAT)
		{
			fout = append_trunc(tmp);
			if (fout == 2)
				break ;
		}
		else if (tmp->e_type == LESS || tmp->e_type == LESSANDLESS)
		{
			fin = open_read(tmp, head, exe);
			if (fin == -1 || fin == 2)
				break ;
		}
		tmp = tmp->next;
	}
	dup_files(exe, fin, fout);
}
