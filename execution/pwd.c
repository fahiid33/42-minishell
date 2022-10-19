/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 02:33:17 by fahd              #+#    #+#             */
/*   Updated: 2022/10/10 02:29:16 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		ft_putstr_fd("pwd: cannot get current working directory\n", 2);
		g_vars.exit_status = 1;
		return (g_vars.exit_status);
	}
	ft_putstr_fd(cwd, 1);
	ft_putstr_fd("\n", 1);
	g_vars.exit_status = 0;
	return (g_vars.exit_status);
}
