/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 04:53:30 by fahd              #+#    #+#             */
/*   Updated: 2022/10/27 05:36:57 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_cmd(t_parse *cmd)
{
	int		u;
	char	*save;

	u = 0;
	save = ft_strdup(cmd->cmd);
	if (!cmd->cmd)
		return ;
	if (is_upper_case(cmd->cmd))
	{
		u = 1;
		cmd->cmd = ft_lowercase(cmd->cmd);
	}
	if (builtins_cases(cmd) && ft_strcmp(cmd->cmd, "echo") != 0 && u)
	{
		cmd->cmd = save;
		return ;
	}
	if (!ft_strcmp(cmd->cmd, "echo") && u)
	{
		cmd->cmd = save;
		return ;
	}
}

void	wrong_cmd(char *cmd)
{
	char	*error;

	error = strerror(errno);
	write(2, "minishell: ", ft_strlen("minishell: "));
	if (errno == 2)
	{
		if (ft_int_strchr(cmd, '/') != -1)
		{
			write(2, error, ft_strlen(error));
			write(2, "\n", 1);
			g_vars.exit_status = 127;
		}
		else
		{
			write(2, cmd, ft_strlen(cmd));
			write(2, ": command not found\n", 21);
			g_vars.exit_status = 127;
		}
		exit(g_vars.exit_status);
	}
	else if (errno == 13 || errno == 21)
		wrong_cmd_helper(error, 0);
	else
		wrong_cmd_helper(error, 1);
}

int	count_env(t_env **env)
{
	t_env	*tmp;
	int		i;

	i = 0;
	tmp = *env;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

char	**env_to_tab(t_env **env)
{
	int		i;
	int		len;
	char	**tab;
	t_env	*tmp;

	i = 0;
	len = count_env(env);
	tmp = (*env);
	tab = (char **)f_malloc(sizeof(char *) * (len + 1));
	while (tmp)
	{
		tab[i] = join_3_str(tmp->key, "=", tmp->val);
		i++;
		tmp = tmp->next;
	}
	tab[i] = NULL;
	return (tab);
}

void	execute(t_parse *command, t_env **env)
{
	char	*path;
	char	**new_env;

	new_env = env_to_tab(env);
	path = get_path(command->cmd, new_env);
	if (!command->cmd)
	{
		g_vars.exit_status = 0;
		exit(g_vars.exit_status);
	}
	if (command->cmd[0] == '/' && access(command->cmd, F_OK) != 0)
		wrong_cmd(command->cmd);
	if (execve(path, command->argv, new_env) == -1)
		wrong_cmd(command->cmd);
}
