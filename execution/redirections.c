/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoubare <amoubare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/30 18:45:13 by fahd              #+#    #+#             */
/*   Updated: 2022/10/28 05:21:28 by amoubare         ###   ########.fr       */
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
	open_redir(cmd, 0);
	dup_pipes(cmd, in, index, fd);
}

int	append_trunc(t_redir *redir)
{
	int	fout;

	fout = 1;
	if (redir->file[0] == '*' || redir->file[0] == '\0')
		return (ambg_redir(redir->file));
	if (redir->e_type == GREAT)
	{
		if (open_files(redir, 1) == -1)
			return (2);
		fout = redir->fdout;
	}
	else
	{
		if (open_files(redir, 2) == -1)
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
		if (access(redir->file, F_OK) != 0)
		{
			if (read_error(redir->file, exec) == -1)
				return (-1);
			exit(g_vars.exit_status);
		}
		else
		{
			fin = open_files(redir, 3);
			if (fin == -1)
				return (-1);
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
