/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 04:53:30 by fahd              #+#    #+#             */
/*   Updated: 2022/10/30 20:49:43 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	wrong_cmd1(char *cmd, char *error)
{
	if (!ft_strcmp(cmd, "/"))
	{
		ft_putstr_fd("minishell: /: is a directory\n", 2);
		g_vars.exit_status = 126;
	}
	else if (ft_int_strchr(cmd, '/') != -1)
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
}

void	wrong_cmd(char *cmd)
{
	char	*error;

	error = strerror(errno);
	write(2, "minishell: ", ft_strlen("minishell: "));
	if (errno == 2)
	{
		wrong_cmd1(cmd, error);
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
	if (command->cmd[0] == '/' || (command->cmd[0] == '.'
			&& command->cmd[1] == '/'))
	{
		if (access(command->cmd, F_OK) != 0)
			wrong_cmd(command->cmd);
	}
	if (execve(path, command->argv, new_env) == -1)
		wrong_cmd(command->cmd);
}
