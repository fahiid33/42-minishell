/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 02:32:47 by fahd              #+#    #+#             */
/*   Updated: 2022/11/01 22:10:14 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	change_pwd(char *oldpwd)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	save_address(cwd);
	update_export(&g_vars.my_env, "PWD", '=', cwd);
	update_export(&g_vars.my_env, "OLDPWD", '=', oldpwd);
}

int	home_cd(void)
{
	char	*home;
	char	*cwd;

	home = my_getenv(g_vars.my_env, "HOME");
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
			_errors(1, o_pwd);
	}
}

int	cd_tilde(t_parse *head)
{
	char	*dir;
	char	*old_pwd;

	old_pwd = getcwd(NULL, 0);
	save_address(old_pwd);
	if (!strcmp(head->argv[0], "~"))
		return (home_cd());
	dir = strchr(head->argv[0], '/');
	if (dir)
	{
		dir++;
		if (home_cd() == 1)
			return (1);
		if (!chdir(dir))
		{
			change_pwd(old_pwd);
			g_vars.exit_status = 0;
		}
		else
			_errors(1, head->argv[0]);
	}
	else
		_errors(1, head->argv[0]);
	return (g_vars.exit_status);
}

int	cd(t_parse *head)
{
	char	*pwd;
	char	*cwd;

	pwd = getcwd(NULL, 0);
	save_address(pwd);
	if (head->argv[0] && (!strcmp(head->argv[0], "~")
			|| head->argv[0][0] == '~'))
		return (cd_tilde(head));
	if (!head->argv[0] || head->argv[0][0] == '\0')
		return (home_cd());
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
