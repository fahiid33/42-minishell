/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 02:33:17 by fahd              #+#    #+#             */
/*   Updated: 2022/10/25 01:57:33 by fstitou          ###   ########.fr       */
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

char	*ft_upper_case(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (str);
	while (str[i])
	{
		if (str[i] >= 'a' && str[i] <= 'z')
			str[i] -= 32;
		i++;
	}
	return (str);
}
