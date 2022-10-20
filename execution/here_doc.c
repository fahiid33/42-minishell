/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 02:29:19 by fahd              #+#    #+#             */
/*   Updated: 2022/10/20 01:07:46 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*random_filename(void)
{
	static int	nb_file = 0;
	char		*filename;
	char		*filename_nb;

	nb_file++;
	filename_nb = ft_itoa(nb_file);
	filename = ft_strjoino("/tmp/file", filename_nb, -1);
	free(filename_nb);
	return (filename);
}

int	heredoc_fd(int fd, char *filename)
{
	close(fd);
	if (g_vars.exit_sig != -27)
		fd = open(filename, O_RDONLY, 0644);
	return (fd);
}

int	open_heredoc(char *limiter, char *filename)
{
	int		fd;
	char	*doc;
	int		*seq;

	fd = open(filename, O_RDWR | O_TRUNC | O_CREAT, 0644);
	while (1)
	{
		if (isatty(0))
			doc = readline(">");
		else
			break ;
		if (!doc)
			break ;
		if (!ft_strcmp(doc, limiter))
		{
			if (doc)
				free(doc);
			break ;
		}
		seq = f_malloc(sizeof(int) * ft_strlen(doc));
		fill_sequences(ft_strlen(doc), seq);
		doc = expand_dollar(doc, seq, 1);
		ft_putstr_fd(doc, fd);
		ft_putchar_fd('\n', fd);
		if (doc)
			free(doc);
	}
	return (heredoc_fd(fd, filename));
}

void	read_heredocs(t_parse *command)
{
	t_redir	*redir;
	t_parse	*cmd;
	char	*tmpfile;
	int		fd;

	cmd = command;
	g_vars.exit_sig = 27;
	fd = dup(0);
	while (cmd)
	{
		redir = cmd->redir;
		while (redir)
		{
			if (redir->e_type == LESSANDLESS && redir->file)
			{
				tmpfile = random_filename();
				redir->fdin = open_heredoc(redir->file, tmpfile);
				unlink(tmpfile);
				free(tmpfile);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	dup2(fd, 0);
}
