/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahd <fahd@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 05:11:48 by fahd              #+#    #+#             */
/*   Updated: 2022/09/21 22:52:46 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_child(t_parse *head, t_env **env)
{
	if (head->cmd && builtins_cases(head))
	{
		g_vars.exit_status = exec_builtins(head, env);
		exit (g_vars.exit_status);
	}
	else
		execute(head, env);
}

void	__child(t_parse *cmd, t_env **env)
{
	int	fd[2];
	int	i;
	int	in;

	i = 0;
	in = 0;
	while (cmd->next)
	{
		check_cmd(cmd);
		pipe(fd);
		g_vars.pid = fork();
		if (!g_vars.pid)
		{
			pipe_redir(cmd, in, i, fd);
			exec_child(cmd, env);
		}
		close(fd[1]);
		if (i > 0)
			close(in);
		in = fd[0];
		cmd = cmd->next;
		i++;
	}
}

void	exec_simple_cmd(t_parse *cmd, t_env **env)
{
	int	fds[2];

	fds[0] = dup(0);
	fds[1] = dup(1);
	open_redir(cmd, 0);
	g_vars.exit_status = exec_builtins(cmd, env);
	dup2(fds[0], 0);
	dup2(fds[1], 1);
}

void	supervisor(void)
{
	int	status;

	status = 0;
	g_vars.exit_sig = 0;
	waitpid(g_vars.pid, &status, 0);
	while (wait(NULL) > 0)
		;
	if (WIFEXITED(status))
		g_vars.exit_status = WEXITSTATUS(status);
}

void	exec_pipeline(t_parse *commands, t_env **env)
{
	t_parse	*head;

	head = commands;
	if (head && head->cmd)
	{
		if (simple_cmd(head))
		{
			exec_simple_cmd(head, env);
			return ;
		}
	}
	__child(head, env);
	supervisor();
}
