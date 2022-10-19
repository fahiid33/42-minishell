/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 23:24:17 by fstitou           #+#    #+#             */
/*   Updated: 2022/10/10 02:29:16 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_lowercase(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (str);
	while (str[i])
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
		i++;
	}
	return (str);
}

int	simple_cmd(t_parse *cmd)
{
	if (builtins_cases(cmd) && !cmd->next->cmd)
		return (1);
	return (0);
}

int	builtins_cases(t_parse *cmd)
{
	if (!ft_strcmp(cmd->cmd, "echo") || !ft_strcmp(cmd->cmd, "cd")
		|| !ft_strcmp(cmd->cmd, "pwd") || !ft_strcmp(cmd->cmd, "export")
		|| !ft_strcmp(cmd->cmd, "unset") || !ft_strcmp(cmd->cmd, "env")
		|| !ft_strcmp(cmd->cmd, "exit"))
		return (1);
	return (0);
}

int	exec_builtins(t_parse *head, t_env **my_env)
{
	head->argv++;
	if (!strcmp(head->cmd, "cd"))
		return (cd(head, (*my_env)));
	else if (!strcmp(head->cmd, "env"))
		return (env());
	else if (!strcmp(head->cmd, "export"))
		return (export(head));
	else if (!strcmp(head->cmd, "pwd"))
		return (pwd());
	else if (!strcmp(head->cmd, "exit"))
		return (my_exit(head));
	else if (!strcmp(head->cmd, "echo"))
		return (echo(head));
	else if (!strcmp(head->cmd, "unset"))
		return (unset(head));
	head->argv--;
	return (0);
}
