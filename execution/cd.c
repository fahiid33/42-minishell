/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 02:32:47 by fahd              #+#    #+#             */
/*   Updated: 2022/10/10 02:29:16 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_minus(char *arg, char c)
{
	int	i;

	i = 1;
	while (arg[i] && arg[i] == c)
		i++;
	if (!arg[i])
		return (1);
	else
		return (0);
}

void	change_pwd(char *pwd)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	update_export(&g_vars.my_env, "PWD", '=', cwd);
	update_export(&g_vars.my_env, "OLDPWD", '=', pwd);
}

int	home_cd(t_env *env)
{
	char	*home;
	char	*cwd;

	home = my_getenv(env, "HOME");
	cwd = getcwd(NULL, 0);
	if (!home)
	{
		ft_putstr_fd("cd: HOME not set\n", 2);
		g_vars.exit_status = 1;
		return (g_vars.exit_status);
	}
	if (!chdir(home))
	{
		update_export(&g_vars.my_env, "PWD", '=', home);
		update_export(&g_vars.my_env, "OLDPWD", '=', cwd);
		g_vars.exit_status = 0;
	}
	else
	{
		ft_putstr_fd("minishell: cd: ", 2);
		g_vars.exit_status = 1;
	}
	return (g_vars.exit_status);
}

void	cd_minus(char *pwd)
{
	char	*o_pwd;

	o_pwd = my_getenv(g_vars.my_env, "OLDPWD");
	if (o_pwd)
	{
		ft_putstr_fd(o_pwd, 1);
		ft_putchar_fd('\n', 1);
		update_export(&g_vars.my_env, "PWD", '=', o_pwd);
		update_export(&g_vars.my_env, "OLDPWD", '=', pwd);
		chdir(o_pwd);
		g_vars.exit_status = 0;
	}
}

int	cd(t_parse *head, t_env *env)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!head->argv[0] || !strcmp(head->argv[0], "~")
		|| head->argv[0][0] == '\0')
		return (home_cd(env));
	else if (!strcmp(head->argv[0], "-"))
		cd_minus(pwd);
	else if (!chdir(head->argv[0]))
	{
		change_pwd(pwd);
		g_vars.exit_status = 0;
	}
	else
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(head->argv[0], 2);
		ft_putstr_fd(" No such file or directory", 2);
		ft_putchar_fd('\n', 2);
		g_vars.exit_status = 1;
	}
	return (g_vars.exit_status);
}
