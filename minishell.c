/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 18:45:32 by fahd              #+#    #+#             */
/*   Updated: 2022/10/31 00:17:44 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_shell(void)
{
	printf("exit\n");
	exit(g_vars.exit_status);
}

void	init_gs_and_c_signal(void)
{
	g_vars.line = NULL;
	g_vars.exit_sig = 0;
	g_vars.g_err = 0;
	g_vars.flag = 0;
	c_signal();
}

void	mini(t_parse *cmds)
{
	if (!g_vars.g_err)
	{
		read_heredocs(cmds);
		if (g_vars.exit_sig != -27)
			exec_pipeline(cmds, &g_vars.my_env);
	}
}

int	main(int ac, char *av[], char **env)
{
	t_parse	*commands;
	t_token	*tokens;

	commands = NULL;
	tokens = NULL;
	(void)ac;
	(void)av;
	g_vars.index = 0;
	init_env(env);
	while (1)
	{
		init_gs_and_c_signal();
		g_vars.line = readline("minishell-1.0$ ");
		if (!g_vars.line)
			exit_shell();
		if (only_enter())
			continue ;
		commands = init_cmd();
		tokens = init_create_tokens(tokens, g_vars.line);
		tokens = parsing(tokens);
		fill_tparse(tokens, &commands);
		add_history(g_vars.line);
		mini(commands);
		free(g_vars.line);
	}
}
