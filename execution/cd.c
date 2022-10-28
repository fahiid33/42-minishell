/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 02:32:47 by fahd              #+#    #+#             */
/*   Updated: 2022/10/27 14:35:01 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_minus(char *arg, char c)
{
	int	i;

	i = 1;
	while (arg[i] && arg[i] == c)
		i++;
	if (!arg[i] && arg[0] == '-')
		return (1);
	else
		return (0);
}

void	change_pwd(char *oldpwd)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	save_address(cwd);
	update_export(&g_vars.my_env, "PWD", '=', cwd);
	update_export(&g_vars.my_env, "OLDPWD", '=', oldpwd);
}

int	home_cd(t_env *env)
{
	char	*home;
	char	*cwd;

	home = my_getenv(env, "HOME");
	cwd = getcwd(NULL, 0);
	save_address(cwd);
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
		if (!chdir(o_pwd))
		{
			ft_putstr_fd(o_pwd, 1);
			ft_putchar_fd('\n', 1);
			update_export(&g_vars.my_env, "PWD", '=', o_pwd);
			update_export(&g_vars.my_env, "OLDPWD", '=', pwd);
			g_vars.exit_status = 0;
		}
		else
		{
			ft_putstr_fd("minishell: cd: ", 2);
			ft_putstr_fd(o_pwd, 2);
			ft_putstr_fd(" No such file or directory", 2);
			ft_putchar_fd('\n', 2);
			g_vars.exit_status = 1;
		}
	}
}

int	cd(t_parse *head, t_env *env)
{
	char	*pwd;
	char	*cwd;

	pwd = getcwd(NULL, 0);
	save_address(pwd);
	if (!head->argv[0] || !strcmp(head->argv[0], "~")
		|| head->argv[0][0] == '\0')
		return (home_cd(env));
	else if (!strcmp(head->argv[0], "-"))
		cd_minus(pwd);
	else if (!chdir(head->argv[0]))
	{
		cwd = getcwd(NULL, 0);
		save_address(cwd);
		if (cwd)
			change_pwd(pwd);
		else
			ft_putstr_fd("cd: error retrieving current directory\n", 2);
		g_vars.exit_status = 0;
	}
	else
		_errors(1, head->argv[0]);
	return (g_vars.exit_status);
}
